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

from infinityroom.connect_to_controllers import GetIP
from artstdweb.constants import const
import requests

class SendToNodeMCU:
    def send(target, data):
        count = 2
        while(count > 0):
            count -= 1
            try:
                adr = GetIP.getAdr(const.hMap[target])
                res = requests.get(adr+target, params=data)
                return res.text
            #except ConnectionError:
            except :
                #Not able to reach infinty room
                #Refresh the address
                GetIP.updateIp(const.hMap[target])
        #End while
        return 'Not able to connect to ' + const.hMap[target]
    
def sendToControllers(target, data):
    #Decide for which send to call i.e. NodeMCU, or any other device
    return SendToNodeMCU.send(target, data)
    
    
    
    