#pragma once

// Windows
#include <ntifs.h>
#include <ntddk.h>
#include <ntdef.h>
#include <ntstatus.h>
#include <ntstrsafe.h>
#include <ntimage.h>

// Project
#include <defs.hpp>
#include <shared.hpp>

#include <impl/nt.hpp>
#include <impl/crt.hpp>
#include <impl/scanner.hpp>

#include <kernel/ctx.hpp>
#include <kernel/modules.hpp>

#include <physical/physical.hpp>
#include <physical/cr3.hpp>

#include <comm/handler.hpp>