/*
 * Copyright (c) 2025, Niccolo Antonelli-Dziri <niccolo.antonelli-dziri@protonmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/String.h>

namespace Core {

// This class is responsible for the distribution of notifications across all platforms by calling
// the right methods defined in their specific files
class NotificationHandler {
public:
    static void notify(String const& title);
};

}
