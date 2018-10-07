from django.shortcuts import render
from django.http import HttpResponse, HttpResponseRedirect

from infinityroom.process_request import ProcessRequest as pr

# Create your views here.
def index(req):
    status, data = pr.home(req)
    if(status):
        #return render(req, 'inf_control.html', data)
        return render(req, 'main_page.html', data)
    else:
        return HttpResponse(data)

def install(req):
    added = pr.install(req)
    return HttpResponse(added)

def changeTheme(req):
    status, data = pr.changeTheme(req)
    if(status):
        #return redirect('index')
        return HttpResponseRedirect("/#infrm")
    else:
        return HttpResponse(data)

def shaeed(req):
    return render(req, 'main_page_2.html')

def changeDevice(req):
    status, data = pr.changeDevice(req)
    if(status):
        #return redirect('index')
        return HttpResponseRedirect("/#main-cntrl")
    else:
        return HttpResponse(data)
    
def infRemote(req):
    status, data = pr.infRemote(req)

