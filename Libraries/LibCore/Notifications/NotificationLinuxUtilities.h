/*
 * Copyright (c) 2025, Niccolo Antonelli-Dziri <niccolo.antonelli-dziri@protonmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/OwnPtr.h>
#include <AK/String.h>
#include <QDBusInterface>
#include <QString>

namespace Core {

QString ak_string_to_qstring(String const& str);

class NotificationLinuxUtilities {
public:
    NotificationLinuxUtilities()
        : m_notification_interface(nullptr)
    {
        this->initialize_dbus();
    }

    bool is_dbus_connected() { return m_notification_interface && m_notification_interface->isValid(); }

    // FIXME: Currently has debug statements and errors should be handled better.
    // NB: This function calls the methods "Notify" from "org.freedesktop.Notifications".
    // https://specifications.freedesktop.org/notification/latest-single/#command-notify
    unsigned notify(String const& title);

    // FIXME: Currently has debug statements and errors should be handled better
    void initialize_dbus();

private:
    OwnPtr<QDBusInterface> m_notification_interface;
};

}
