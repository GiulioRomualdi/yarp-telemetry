# Copyright (C) 2006-2020 Istituto Italiano di Tecnologia (IIT)
# All rights reserved.
#
# This software may be modified and distributed under the terms of the
# BSD-3-Clause license. See the accompanying LICENSE file for details.

project(telemetry)

add_subdirectory(src)

include(TelemetryInstallBasicPackageFiles)
telemetry_install_basic_package_files(telemetry
                                      DEPENDENCIES ${telemetry_PUBLIC_DEPS}
                                      PRIVATE_DEPENDENCIES ${telemetry_PRIVATE_DEPS})
