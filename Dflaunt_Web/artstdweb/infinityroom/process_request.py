# -*- coding: utf-8 -*-

from infinityroom.install import createThemes
from infinityroom.models import LEDThemes, Device
from infinityroom.send_to_controllers import sendToControllers
from artstdweb.constants import const

class ProcessRequest:
    def home(req = None):
        #Get all the available themes
        themes = LEDThemes.objects.all()
        
        devices = Device.objects.all()
        if(len(devices) == 0):
            #Nothing installed
            return (False, 'Nothing found, Please run Install.')
        
        #for dev in devices:
        #    print(dev)
        
        selectedTheme = 0
        devInf = [x for x in devices if x.devType == const.DEV_IR][0]
        if('selectedTheme' in devInf.other):
            selectedTheme = devInf.other['selectedTheme']
        
        #Get the selected theme  details
        themeDetails = [t for t in themes if t.themeId == selectedTheme][0]
        
        data = {'themes' : themes,
        'selectedTheme' : selectedTheme,
        'selectedThemeName' : themeDetails.themeName,
        'brt' : themeDetails.themeOption['brt'], #0 If brightness is not available for selected theme
        'spd' : themeDetails.themeOption['spd'],
        'clr' : themeDetails.themeOption['clr'],
        'brtV' : themeDetails.themeBrt,          #Brightness value
        'spdV' : themeDetails.themeSpd,
        'spdVMax' : themeDetails.themeSpdMax,
        'spdVMin' : themeDetails.themeSpdMin,
        'clrV' : themeDetails.themeClr,
        'devices' : devices
        }
        
        return (True, data)
    #End home
    
    def changeTheme(req):
        #get the GET arguments
        theme = req.GET.get('theme', None)
        brt = req.GET.get('brt', None)
        spd = req.GET.get('spd', None)
        clr = req.GET.get('clr', None)
        if(clr):
            clr = int(clr, 16)
        
        if(theme is None):
            #Something weent wrong
            return (False, 'Something went wrrong')
        
        #Get the details from DB
        themeDetails = LEDThemes.objects.get(themeId=theme)
        
        if(themeDetails.themeOption['brt'] == 1):
            #Brightness option present
            if(brt is None):
                brt = themeDetails.themeBrt;
            else:
                themeDetails.themeBrt = brt;
        else:
            brt = None
        
        if(themeDetails.themeOption['spd'] == 1):
            #Brightness option present
            if(spd is None):
                #No brightess change received from user
                spd = themeDetails.themeSpd;
            else:
                themeDetails.themeSpd = spd;
        else:
            spd = None
        
        if(themeDetails.themeOption['clr'] == 1):
            #Brightness option present
            if(clr is None):
                #No brightess change received from user
                clr = int(themeDetails.themeClr, 16)
            else:
                themeDetails.themeClr = hex(clr)[2:]
        else:
            clr = None
        
        #Save changes to DB
        themeDetails.save()
        
        #Save the devices DB
        dev = Device.objects.get(devType=const.DEV_IR)
        #print(dev)
        if(brt != None):
            dev.brt = brt
        if(spd != None):
            dev.spd = spd
        if(clr != None):
            dev.clr = hex(clr)[2:]
        
        #State & Theme options
        if(int(theme) > 1):
            dev.state = True
            dev.option = themeDetails.themeOption
        else:
            dev.state = False
    
        #dev.other['selectedTheme'] = theme #Not working
        dc = dev.other
        dc['selectedTheme'] = int(theme)
        dev.other = dc
        
        dev.save()
        
        data = {'thm':theme,
                'brt':brt,
                'spd':spd,
                'clr':clr,
                'apply':True}
        
        #Send command to Controller
        #SendToNodeMCU.send(const.infRoomThmAdr, data)
        res = sendToControllers(const.TAR_IR, data)
        
        return (True, res)
    #End changeTheme
    
    def changeDevice(req):
        #get the GET arguments
        devId = req.GET.get('devid', None)
        brt = req.GET.get('brt', None)
        spd = req.GET.get('spd', None)
        clr = req.GET.get('clr', None)
        if(clr):
            clr = int(clr, 16)
        
        if(devId is None):
            #Something weent wrong
            return (False, 'Something went wrrong')
        
        #Get the details from DB
        dev = Device.objects.get(devId=devId)
        
        #If Device On/Off request is received
        if(brt == None and spd == None and clr == None):
            #Change the device state
            dev.state = dev.state != True
            #If switching ON, read the saved values from db
            if(dev.option['brt'] == 1):
                brt = dev.brt
            if(dev.option['spd'] == 1):
                spd = dev.spd
            if(dev.option['clr'] == 1):
                clr = int(dev.clr, 16)
                
            #Infinity Room LEDs
            if(dev.devType == const.DEV_IR):
                if(dev.state):
                    #ON
                    dc = dev.other
                    dc['selectedTheme'] = dc['prev']
                    dev.other = dc
                else:
                    #Off
                    dc = dev.other
                    dc['prev'] = dc['selectedTheme']
                    dc['selectedTheme'] = 1
                    dev.other = dc
            #End Infinity Room LEDs
        else:
            #Make values none if they are not suitable for device
            #Just a security check, will not be used in normal operation.
            if(dev.option['brt'] != 1):
                brt = None
            elif(brt != None):
                #Save the brightness to DB
                dev.brt = brt
                
            if(dev.option['spd'] != 1):
                spd = None
            elif(spd != None):
                dev.spd = spd
            
            if(dev.option['clr'] != 1):
                clr = None
            elif(clr != None):
                dev.clr = hex(clr)[2:]
                
            #Infinity Room Leds
            if(dev.devType == const.DEV_IR):
                theme = LEDThemes.objects.get(themeId=dev.other['selectedTheme'])
                if(brt):
                    theme.themeBrt = brt
                if(spd):
                    theme.themeSpd = spd
                if(clr):
                    theme.themeClr = hex(clr)[2:]
                theme.save()
        
        #save device changes
        dev.save()
        
        data = {'devid':devId,
                'brt':brt,
                'spd':spd,
                'clr':clr,
                'apply':True}
        
        #Infinity Room Leds theme number
        if(dev.devType == const.DEV_IR):
            data['thm'] = dev.other['selectedTheme']
        
        #Send command to controller
        res = sendToControllers(dev.target, data)
        
        #Redirect to load the page
        return (True, res)
    #End changeDevice
    
    def install(req):
        createThemes()
        
        added = ''
        
        themes = LEDThemes.objects.all()
        for t in themes:
            added += t.themeName + ', '
            
        return added
    #End install
    

    def infRemote(req):
        #get the GET arguments
        key = req.GET.get('key', None)
        res = ''
        if(key == 1):
            #Start the show
            pass
        else:
            #Stop the show
            pass
        
        #Redirect to load the page
        return (True, res)
    #End changeDevice 
    