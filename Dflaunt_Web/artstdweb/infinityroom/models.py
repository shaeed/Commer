from django.db import models
import json

# Create your models here.
class LEDThemes(models.Model):
    themeName = models.CharField(max_length=100)
    themeId = models.IntegerField()
    #Order in which it will be displayed
    themeSNo = models.IntegerField()
    #type - Where to calculate the effects (Node MCU - 0 or in PI - 1)
    themeType = models.IntegerField()
    themeBrt = models.IntegerField()
    themeSpd = models.IntegerField()
    themeSpdMax = models.IntegerField()
    themeSpdMin = models.IntegerField()
    themeClr = models.CharField(max_length=8)
    themeOption_db = models.CharField(max_length=100)
    #True- will be shown to user
    enable = models.BooleanField()
        
    @property
    def themeOption(self):
        return json.loads(self.themeOption_db)

    @themeOption.setter
    def themeOption(self, opt):
        self.themeOption_db = json.dumps(opt)
        
    @themeOption.deleter
    def themeOption(self):
        del self.themeOption_db
      
    def __str__(self):
        return '<{}, {}>'.format(self.themeName, self.themeId)

#==============================================================================
# class GlobalVars(models.Model):
#     selectedTheme = models.IntegerField()
#     tcp_db = models.TextField(blank=True)
#     #num = 1 always
#     gvId = models.IntegerField()
#     
#     @property
#     def tcp(self):
#         #return pickle.load(self.tcp_db)
#         #return base64.b64decode(self.tcp_db)
#         return dill.load(self.tcp_db)
#     
#     @tcp.setter
#     def tcp(self, d):
#         #self.tcp_db = pickle.dumps(d, protocol=pickle.HIGHEST_PROTOCOL)
#         #self.tcp_db = base64.b64encode(d)
#         self.tcp_db = dill.dumps(d)
#==============================================================================

class Settings(models.Model):
    myIp = models.CharField(max_length=20)
    #infinity room ip
    #In the form of dictionary
    #Key - System name like InfinityRoom
    #Value - IP
    sys_ip_db = models.CharField(max_length=99999)
    
    @property
    def sysIp(self):
        print(self.sys_ip_db)
        return json.loads(self.sys_ip_db)
    
    @sysIp.setter
    def sysIp(self, d):
        self.sys_ip_db = json.dumps(d)
        
    @sysIp.deleter
    def sysIp(self):
        del self.sys_ip_db
    
    def __str__(self):
        return 'MyIp:{}, sysIp:{}'.format(self.myIp, self.sysIp)

class Device(models.Model):
    name = models.CharField(max_length=100)
    devId = models.IntegerField()
    #Order in which it will be displayed
    sno = models.IntegerField()
    #type - LED/FAN/Port
    devType = models.CharField(max_length=8)
    #Group - For grouping of devices (List of groups)
    devGrp_db = models.TextField(null=True)
    #Target - Which action have to be taken
    target = models.CharField(max_length=8)
    brt = models.IntegerField()
    spd = models.IntegerField()
    spdMax = models.IntegerField()
    spdMin = models.IntegerField()
    clr = models.CharField(max_length=8)
    option_db = models.CharField(max_length=100) 
    #True- will be shown to user
    enable = models.BooleanField() 
    #True- Switched ON 
    state = models.BooleanField()
    
    #Disctionary object
    other_db = models.TextField()
    
    @property
    def devGrp(self):
        return json.loads(self.devGrp_db)
    @devGrp.setter
    def devGrp(self, d):
        self.devGrp_db = json.dumps(d)
    @devGrp.deleter
    def devGrp(self):
        del self.devGrp_db
    
    @property
    def option(self):
        return json.loads(self.option_db)
    @option.setter
    def option(self, d):
        self.option_db = json.dumps(d)
    @option.deleter
    def option(self):
        del self.option_db
    
    @property
    def other(self):
        return json.loads(self.other_db)
    @other.setter
    def other(self, d):
        self.other_db = json.dumps(d)
    @other.deleter
    def other(self):
        del self.other_db
        
    def __str__(self):
        return str(self.devId) + str(self.name)
    