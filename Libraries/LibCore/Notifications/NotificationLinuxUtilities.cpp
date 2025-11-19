/*
 * Copyright (c) 2025, Niccolo Antonelli-Dziri <niccolo.antonelli-dziri@protonmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/String.h>
#include <LibCore/Notifications/NotificationLinuxUtilities.h>
#include <QString>

namespace Core {

QString ak_string_to_qstring(String const& str)
{
    StringView view = str.bytes_as_string_view();
    return QString::fromUtf8(view.characters_without_null_termination(), (long long)view.length());
}

void NotificationLinuxUtilities::initialize_dbus()
{
    QDBusConnection bus = QDBusConnection::sessionBus();
    if (!bus.isConnected()) {
        dbgln("Failed to connect to the D-Bus session bus");
        m_notification_interface = nullptr;
        return;
    }
    OwnPtr<QDBusInterface> notification_interface = make<QDBusInterface>(
        "org.freedesktop.Notifications",  // service name
        "/org/freedesktop/Notifications", // path
        "org.freedesktop.Notifications",  // interface
        bus);
    if (!notification_interface->isValid()) {
        dbgln("Failed to create D-Bus interface for notifications");
        dbgln("Error: {}", qPrintable(bus.lastError().message()));
        m_notification_interface = nullptr;
        return;
    }
    m_notification_interface = move(notification_interface);
}

unsigned NotificationLinuxUtilities::notify(String const& title)
{
    // https://specifications.freedesktop.org/notification/latest-single/#id-1.10.3.3.4
    QVariantList args;
    args << "Ladybird";                  // app_name
    args << (unsigned)0;                 // replaces_id
    args << "";                          // app_icon
    args << ak_string_to_qstring(title); // summary
    args << "";                          // body
    args << QStringList();               // actions
    args << QVariantMap();               // hints
    args << -1;                          // expire_timeout

    QDBusMessage reply = m_notification_interface->callWithArgumentList(QDBus::AutoDetect, "Notify", args);
    if (reply.type() == QDBusMessage::ErrorMessage) {
        dbgln("D-Bus error calling Notify: {}", qPrintable(reply.errorMessage()));
        return 0;
    }
    unsigned notification_id = reply.arguments().at(0).toUInt();
    dbgln("Notification sent successfully with ID: {}", notification_id);
    return notification_id;
}

}
