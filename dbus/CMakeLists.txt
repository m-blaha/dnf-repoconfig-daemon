SET(SYSTEMD_UNIT_DIR /usr/lib/systemd/system)
SET(DBUS_SERVICES_DIR /usr/share/dbus-1/system-services)
SET(DBUS_CONFIG_DIR ${SYSCONFDIR}/dbus-1/system.d)

install (
    FILES "dnf-repoconfig-daemon.service"
    DESTINATION ${SYSTEMD_UNIT_DIR}
)

install (
    FILES "org.rpm.dnf.v1.rpm.RepoConf.service"
    DESTINATION ${DBUS_SERVICES_DIR}
)

install (
    FILES "org.rpm.dnf.v1.conf"
    DESTINATION ${DBUS_CONFIG_DIR}
)
