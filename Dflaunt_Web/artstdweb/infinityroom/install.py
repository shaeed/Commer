# -*- coding: utf-8 -*-

from infinityroom.models import LEDThemes, Device, Settings
from artstdweb.constants import const

def createThemes():
    #Delete old ones
    a = LEDThemes.objects.all()
    for t in a:
        t.delete()
        
    #Off
    led = LEDThemes(
            themeName = 'Off',
            themeId = 1,
            themeSNo = 1,
            themeType = 0,
            themeBrt = 0,
            themeSpd = 0,
            themeSpdMax = 100,
            themeSpdMin = 0,
            themeClr = 'NA',
            themeOption = {'brt':0, 'spd':0, 'clr':0},
            enable = True)
    led.save()
    
    #Rainbow
    led = LEDThemes(
            themeName = 'Rainbow',
            themeId = 3,
            themeSNo = 3,
            themeType = 0,
            themeBrt = 250,
            themeSpd = 7,
            themeSpdMax = 10,
            themeSpdMin = 1,
            themeClr = 'NA',
            themeOption = {'brt':1, 'spd':1, 'clr':0},
            enable = True)
    led.save()
    
    #Strobe
    led = LEDThemes(
            themeName = 'Strobe',
            themeId = 4,
            themeSNo = 4,
            themeType = 0,
            themeBrt = 250,
            themeSpd = 50,
            themeSpdMax = 100,
            themeSpdMin = 0,
            themeClr = '3515FF',
            themeOption = {'brt':1, 'spd':1, 'clr':1},
            enable = True)
    led.save()
    
    #Theme_Strobe2
    led = LEDThemes(
            themeName = 'Strobe 2',
            themeId = 13,
            themeSNo = 13,
            themeType = 0,
            themeBrt = 250,
            themeSpd = 10,
            themeSpdMax = 100,
            themeSpdMin = 0,
            themeClr = 'FFFFFF',
            themeOption = {'brt':1, 'spd':1, 'clr':1},
            enable = True)
    led.save()
    
    #All white
    led = LEDThemes(
            themeName = 'All White',
            themeId = 5,
            themeSNo = 5,
            themeType = 0,
            themeBrt = 250,
            themeSpd = 10,
            themeSpdMax = 100,
            themeSpdMin = 0,
            themeClr = 'FFFFFF',
            themeOption = {'brt':1, 'spd':0, 'clr':0},
            enable = True)
    led.save()
    
    #Colorful
    led = LEDThemes(
            themeName = 'Colorful',
            themeId = 6,
            themeSNo = 6,
            themeType = 0,
            themeBrt = 250,
            themeSpd = 10,
            themeSpdMax = 100,
            themeSpdMin = 0,
            themeClr = '3515FF',
            themeOption = {'brt':1, 'spd':0, 'clr':1},
            enable = True)
    led.save()
    
    #Theme_FadeInFadeOut
    led = LEDThemes(
            themeName = 'Fade In-Out',
            themeId = 12,
            themeSNo = 12,
            themeType = 0,
            themeBrt = 250,
            themeSpd = 10,
            themeSpdMax = 100,
            themeSpdMin = 0,
            themeClr = 'FFFFFF',
            themeOption = {'brt':1, 'spd':0, 'clr':1},
            enable = True)
    led.save()
    
    #Fade
#    led = LEDThemes(
#            themeName = 'Fade In-Out 2',
#            themeId = 7,
#            themeSNo = 7,
#            themeType = 0,
#            themeBrt = 80,
#            themeSpd = 10,
#            themeSpdMax = 100,
#            themeSpdMin = 0,
#            themeClr = '3515FF',
#            themeOption = {'brt':1, 'spd':1, 'clr':1},
#            enable = True)
#    led.save()
    
#    #Night Sky
#    led = LEDThemes(
#            themeName = 'Night Sky',
#            themeId = 10,
#            themeSNo = 10,
#            themeType = 1,
#            themeBrt = 80,
#            themeSpd = 150,
#            themeSpdMax = 300,
#            themeSpdMin = 75,
#            themeClr = 'FFFFFF',
#            themeOption = {'brt':0, 'spd':1, 'clr':0},
#            enable = True)
#    led.save()
#    
#    #THEME_NIGHTSKY2
#    led = LEDThemes(
#            themeName = 'Colorful Night',
#            themeId = 16,
#            themeSNo = 16,
#            themeType = 0,
#            themeBrt = 80,
#            themeSpd = 150,
#            themeSpdMax = 300,
#            themeSpdMin = 75,
#            themeClr = 'FFFFFF',
#            themeOption = {'brt':1, 'spd':1, 'clr':0},
#            enable = True)
#    led.save()
    
#    #Theme_RGBLoop
#    led = LEDThemes(
#            themeName = 'RGB Loop',
#            themeId = 11,
#            themeSNo = 11,
#            themeType = 0,
#            themeBrt = 80,
#            themeSpd = 10,
#            themeSpdMax = 100,
#            themeSpdMin = 0,
#            themeClr = 'FFFFFF',
#            themeOption = {'brt':1, 'spd':1, 'clr':0},
#            enable = True)
#    led.save()
    
#    #Theme_CylonBounce
#    led = LEDThemes(
#            themeName = 'Cylon Bounce',
#            themeId = 14,
#            themeSNo = 14,
#            themeType = 0,
#            themeBrt = 80,
#            themeSpd = 10,
#            themeSpdMax = 100,
#            themeSpdMin = 0,
#            themeClr = 'FFFFFF',
#            themeOption = {'brt':1, 'spd':1, 'clr':1},
#            enable = True)
#    led.save()
#    
#    #Theme_NewKITT
#    led = LEDThemes(
#            themeName = 'Chase Showdown',
#            themeId = 15,
#            themeSNo = 15,
#            themeType = 0,
#            themeBrt = 80,
#            themeSpd = 10,
#            themeSpdMax = 100,
#            themeSpdMin = 0,
#            themeClr = 'FFFFFF',
#            themeOption = {'brt':1, 'spd':1, 'clr':1},
#            enable = True)
#    led.save()
    
    #Theme_Sparkle
    led = LEDThemes(
            themeName = 'Sparkle',
            themeId = 17,
            themeSNo = 17,
            themeType = 0,
            themeBrt = 250,
            themeSpd = 10,
            themeSpdMax = 300,
            themeSpdMin = 0,
            themeClr = 'FFFFFF',
            themeOption = {'brt':1, 'spd':1, 'clr':1},
            enable = True)
    led.save()
    
#    #Theme_SnowSparkle
#    led = LEDThemes(
#            themeName = 'Snow Sparkle',
#            themeId = 18,
#            themeSNo = 18,
#            themeType = 0,
#            themeBrt = 80,
#            themeSpd = 10,
#            themeSpdMax = 100,
#            themeSpdMin = 0,
#            themeClr = 'FFFFFF',
#            themeOption = {'brt':1, 'spd':1, 'clr':1},
#            enable = True)
#    led.save()
    
    #Theme_RunningLights
    led = LEDThemes(
            themeName = 'Running',
            themeId = 19,
            themeSNo = 19,
            themeType = 0,
            themeBrt = 250,
            themeSpd = 10,
            themeSpdMax = 150,
            themeSpdMin = 4,
            themeClr = 'FFFFFF',
            themeOption = {'brt':1, 'spd':1, 'clr':1},
            enable = True)
    led.save()
    
#    #Theme_colorWipe
#    led = LEDThemes(
#            themeName = 'Color Wipe',
#            themeId = 20,
#            themeSNo = 20,
#            themeType = 0,
#            themeBrt = 80,
#            themeSpd = 10,
#            themeSpdMax = 50,
#            themeSpdMin = 2,
#            themeClr = 'FFFFFF',
#            themeOption = {'brt':1, 'spd':1, 'clr':1},
#            enable = True)
#    led.save()
    
    #Theme_theaterChaseRainbow
    led = LEDThemes(
            themeName = 'Theater Chase Rainbow',
            themeId = 21,
            themeSNo = 21,
            themeType = 0,
            themeBrt = 250,
            themeSpd = 20,
            themeSpdMax = 100,
            themeSpdMin = 50,
            themeClr = 'FFFFFF',
            themeOption = {'brt':1, 'spd':1, 'clr':0},
            enable = True)
    led.save()
    
    #Theme_Fire
    led = LEDThemes(
            themeName = 'Fire',
            themeId = 22,
            themeSNo = 22,
            themeType = 0,
            themeBrt = 250,
            themeSpd = 10,
            themeSpdMax = 10,
            themeSpdMin = 2,
            themeClr = 'FFFFFF',
            themeOption = {'brt':1, 'spd':1, 'clr':0},
            enable = True)
    led.save()
    
    #Theme_meteorRain
    led = LEDThemes(
            themeName = 'Meteor Rain',
            themeId = 23,
            themeSNo = 23,
            themeType = 0,
            themeBrt = 250,
            themeSpd = 10,
            themeSpdMax = 15,
            themeSpdMin = 1,
            themeClr = 'FFFFFF',
            themeOption = {'brt':1, 'spd':1, 'clr':1},
            enable = True)
    led.save()
    
    #Theme_HueEffect
    led = LEDThemes(
            themeName = 'Hue Effect',
            themeId = 24,
            themeSNo = 24,
            themeType = 0,
            themeBrt = 250,
            themeSpd = 10,
            themeSpdMax = 100,
            themeSpdMin = 0,
            themeClr = 'FFFFFF',
            themeOption = {'brt':1, 'spd':0, 'clr':0},
            enable = True)
    led.save()
    
    #Devices
    #Delete old ones
    a = Device.objects.all()
    for t in a:
        t.delete()
    #Infinity Room
    dev = Device(
            name = 'Infinity Room',
            devId = 1,
            sno = 1,
            devType = const.DEV_IR,
            devGrp = [const.GRP_HOME],
            target = const.TAR_IR,
            brt = 0,
            spd = 0,
            spdMax = 100,
            spdMin = 0,
            clr = 0,
            option = {'brt':1, 'spd':0, 'clr':0},
            enable = True,
            state = True,
            other = {'selectedTheme':1})
    dev.save()
    
    #Infinity Room L1
    dev = Device(
            name = 'Earth',
            devId = 2,
            sno = 2,
            devType = const.DEV_LED,
            devGrp = [const.GRP_IR],
            target = const.TAR_IRL,
            brt = 0,
            spd = 0,
            spdMax = 100,
            spdMin = 0,
            clr = 0,
            option = {'brt':0, 'spd':0, 'clr':0},
            enable = True,
            state = True,
            other = {})
    dev.save()
    
    #Infinity Room L2
    dev = Device(
            name = 'Mars',
            devId = 3,
            sno = 3,
            devType = const.DEV_LED,
            devGrp = [const.GRP_IR],
            target = const.TAR_IRL,
            brt = 0,
            spd = 0,
            spdMax = 100,
            spdMin = 0,
            clr = 0,
            option = {'brt':0, 'spd':0, 'clr':0},
            enable = True,
            state = False,
            other = {})
    dev.save()
    
    #Infinity Room L3
    dev = Device(
            name = 'Jupiter',
            devId = 4,
            sno = 4,
            devType = const.DEV_LED,
            devGrp = [const.GRP_IR],
            target = const.TAR_IRL,
            brt = 0,
            spd = 0,
            spdMax = 100,
            spdMin = 0,
            clr = 0,
            option = {'brt':0, 'spd':0, 'clr':0},
            enable = True,
            state = False,
            other = {})
    dev.save()
    
    #Infinity Room L4
    dev = Device(
            name = 'Uranus',
            devId = 5,
            sno = 5,
            devType = const.DEV_LED,
            devGrp = [const.GRP_IR],
            target = const.TAR_IRL,
            brt = 0,
            spd = 0,
            spdMax = 100,
            spdMin = 0,
            clr = 0,
            option = {'brt':0, 'spd':0, 'clr':0},
            enable = True,
            state = True,
            other = {})
    dev.save()
    
    a = Settings.objects.all()
    for t in a:
        t.delete()
    