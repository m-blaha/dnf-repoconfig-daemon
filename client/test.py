#!/usr/bin/env python3

import dbus

bus = dbus.SystemBus()
proxy = bus.get_object('org.rpm.dnf.v1.conf.Repos', '/org/rpm/dnf/v1/conf/Repos')
iface = dbus.Interface(proxy, dbus_interface='org.rpm.dnf.v1.conf.Repos')
for repo in iface.list():
    print(repo['id'])
