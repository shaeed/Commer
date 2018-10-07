# -*- coding: utf-8 -*-
from django.conf.urls import url
from . import views

urlpatterns = [
        url(r'^$', views.index, name='index'),
        url(r'^install$', views.install, name='install'),
        url(r'^theme$', views.changeTheme, name='changeTheme'),
        url(r'^device$', views.changeDevice, name='changeDevice'),
        url(r'^inf-remote$', views.infRemote, name='infRemote'),
        url(r'^shaeed$', views.shaeed, name='shaeed')
        ]