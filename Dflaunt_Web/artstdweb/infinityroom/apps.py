from django.apps import AppConfig
#from infinityroom.models import GlobalVars

class InfinityroomConfig(AppConfig):
    name = 'infinityroom'
    def ready(self):
        print('shaeed khan')
        #g = GlobalVars.objects.all()
        #for a in g:
        #    a.delete()
