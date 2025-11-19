/*
 * Copyright (c) 2025, Niccolo Antonelli-Dziri <niccolo.antonelli-dziri@protonmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Format.h>
#include <LibCore/Notifications/NotificationHandler.h>

namespace Core {

void NotificationHandler::notify(
    String const& title,
    String const& body)
{
    dbgln("HELLO WORLD from UNIMPLEMENTED");
    dbgln(title);
    dbgln(body);
}

}
