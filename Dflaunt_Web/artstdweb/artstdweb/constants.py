# -*- coding: utf-8 -*-

#Infinity room name
class const:
    #Hardware names (Will be returned from the NodeMCU)
    infRoom = 'InfinityRoom'
    
    #Devices type
    DEV_IR = 'ir' #Infinity room 2812b leds
    DEV_LED = 'led'
    DEV_FAN = 'fan'
    DEV_OUTLET = 'outlet'
    
    #Target types (URL on which data will be send)
    TAR_IR = 'ir' #Infinity room 2812b leds
    TAR_IRL = 'irl' #Infinity room hanging lights
    
    #Groups
    GRP_HOME = 'grp_home' #Home
    GRP_IR = 'grp_ir' #Infinity room lights
    
    #Mapping of target to devices
    hMap = {TAR_IR:infRoom,
            TAR_IRL:infRoom}