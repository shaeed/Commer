import requests
import socket
from artstdweb.constants import const
from infinityroom.models import Settings

class GetIP:
    def getAdr(target):
        ip = ''
        sett = Settings.objects.all()
        if(len(sett) < 1):
            #Database is empty
            #This will be executed only once
            ctc = ConnectToControllers()
            myIp = ctc.getMyIp()
            cntrls = ctc.scan()
            dct = {}
            for c in cntrls:
                dct[c[1]] = c[0]
            
            #print(cntrls)            
            s = Settings(myIp=myIp, sysIp=dct)
            s.save()
            ip = dct[target]
        else:
            s = sett[0]
            if(target in s.sysIp):
                ip = s.sysIp[target]
            else:
                ip = ''
        
        add = r'http://{}:{}/'.format(ip, ConnectToControllers.port)
        print(add)
        return add
    #End getAdr
    
    def updateIp(target):
#        sett = Settings.objects.all()[0]
#        ip = ConnectToControllers().getTargetIp(target)
#        if(ip != None):
#            #Target is online
#            sip = sett.sysIp
#            sip[target] = ip
#            sett.sysIp = sip
#            sett.save()
#            return True
#        else:
#            #Target is offline
#            return False
        a = Settings.objects.all()
        for t in a:
            t.delete()
    #End updateIp
    

class ConnectToControllers:
    port = 5045
    timeOut = 3
    
    def connectToIR(self):
        '''For connecting to Infinty room'''
        clients = self.scan()
        #print(clients)
        for dev, ip in clients:
            if(dev == const.infRoom):
                return ip
            
        #raise Exception('Not able to connect to ' + const.infRoom)
        return 'Not able to connect to ' + const.infRoom
        
    
    def getTargetIp(self, target):
        clients = self.scan()
        for dev, ip in clients:
            if(dev == target):
                return ip
        #else:
        #    return None
        print(target + ' Offline.')
        return None
    #End connectTo

    def getMyIp(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.connect(("8.8.8.8", 80))
        return (s.getsockname()[0])
    #End getMyIp
    
    def scan(self):
        '''
        Return the list of tuples
        (ip, ControllerId)
        '''
        myip = self.getMyIp()
        from multiprocessing.dummy import Pool as ThreadPool
        
        base = myip[:myip.rfind('.')+1]
        allIps = [base + str(x) for x in range(1,255)]
        allIps.remove(myip)
        
        pool = ThreadPool(20)
        res = pool.map(self.checkIp, allIps)
        
        return [x for x in res if x]
    #End scan
        
    def checkIp(self, ip):
        try:
            res = requests.get(r'http://{}:{}'.format(ip, self.port), timeout=(self.timeOut, 1))
            #print(ip, res.text)
            return (ip, res.text)
        except:
            return False
        
    def ping(self, ip):
        try:
            requests.get(r'http://{}:{}'.format(ip, self.port), timeout=(self.timeOut, 1))
            return True
        except:
            return False
    #End checkIp    
    
    