/*
 * Copyright (c) 2025, Niccolo Antonelli-Dziri <niccolo.antonelli-dziri@protonmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Format.h>
#include <LibCore/Notifications/NotificationHandler.h>
#include <LibCore/Notifications/NotificationLinuxUtilities.h>

namespace Core {

void NotificationHandler::notify(String const& title)
{
    NotificationLinuxUtilities notification_utilities;
    if (!notification_utilities.is_dbus_connected()) {
        dbgln("ERROR: cannot connect to dbus");
        return;
    }

    unsigned notification_id = notification_utilities.notify(title);
    dbgln("NOTIFICATION SUCCESSFULLY SENT: {}", notification_id);
}

}
