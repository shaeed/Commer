###############################################################################
# Communication Standrad & Protocol
#
# 1) Receive '>>' for every data sent as acknowladge
# 2) Send '\r' in the end of every chunk of data
#
# Command information
# A         - Apply theme
# B 123     - Brightness
# C 1234567 - Color
# L 123     - LED
# S 12      - Speed
# T 12      - Theme
# 
# Example-
# T 1;B 123;S 12;C 123456;A 1; \r

import socket
    
class SendToNodeMCU:

    def send(self, data, tcp):
        st = self.getStringFormat(data)
        tcp.send(st.encode('utf-8'))
    
    def getStringFormat(self, data):
        st = ''
        if('apply' in data):
            st += 'A 1;'
            
        if('brt' in data):
            st += 'B ' + str(data['brt']) + ';'
            
        if('clr' in data):
            st += 'C ' + str(data['clr']) + ';'
            
        if('led' in data):
            st += 'L ' + str(data['led']) + ';'
            
        if('spd' in data):
            st += 'S ' + str(data['spd']) + ';'
            
        if('thm' in data):
            st += 'T ' + str(data['thm']) + ';'
        
        return st + '\r'


class ConnectToNodeMcu:
    port = 5045
    host = ''
    handsake = ('Hello ESP\r').encode('utf-8')
    
    def connect(self):
        #h = self.getNodeMcuIp()
        h = ['192.168.43.119']
        print(h)
        if(h):
            self.host = h[0]
        else:
            raise Exception
        while True:
            try:
                sc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                sc.settimeout(5)
                sc.connect((self.host, self.port))
                print('Connected')
                sc.send(self.handsake)
                res = sc.recv(40).decode()
                print(res)
                #if(res == '>>'):
                if(res):
                    #Success
                    print('returning')
                    return sc
            except:
                print('Exception during TCP connection')
        
    def getMyIp(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.connect(("8.8.8.8", 80))
        return (s.getsockname()[0])
    
    def getNodeMcuIp(self):
        myip = self.getMyIp()
        from multiprocessing.dummy import Pool as ThreadPool
        
        base = myip[:myip.rfind('.')+1]
        allIps = [base + str(x) for x in range(1,255)]
        allIps.remove(myip)
        
        pool = ThreadPool(20)
        res = pool.map(self.checkIpNodeMcu, allIps)
        return [x for x in res if x]
        
        
    def checkIpNodeMcu(self, ip):
        try:
            sc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sc.settimeout(3)
            sc.connect((ip, self.port))
            sc.send(self.handsake)
            sc.recv(40)
            sc.close()
            return ip
        except:
            return False
        
    def handSake(self, sc):
        try:
            sc.send(self.handsake)
            if(sc.recv(40)):
                return True
        except:
            return False

        