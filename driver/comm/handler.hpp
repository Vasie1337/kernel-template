#pragma once
#include <include.hpp>

namespace comm::handler
{
	NTSTATUS HandleData(CommData& Data)
	{
		Data.delivered = true;

		PEPROCESS TargetProcess = nullptr;
		if (!NT_SUCCESS(PsLookupProcessByProcessId(reinterpret_cast<HANDLE>(Data.pid), &TargetProcess)))
		{
			printf("Failed to get process\n");
			return STATUS_UNSUCCESSFUL;
		}

		switch (Data.type)
		{
			case CommType::test:
			{
				printf("Test request\n");

				printf("pid: %d\n", Data.pid);
				printf("address: 0x%llx\n", Data.address);
				printf("buffer: 0x%llx\n", Data.buffer);
				printf("size: %d\n", Data.size);

				return STATUS_SUCCESS;
			}
			case CommType::base:
			{
				printf("Base request\n");

				uint64 TargetBase = reinterpret_cast<uint64>(PsGetProcessSectionBaseAddress(TargetProcess));
				if (!TargetBase)
				{
					printf("Failed to get base\n");
					return STATUS_UNSUCCESSFUL;
				}

				printf("Base: 0x%llx\n", TargetBase);

				Data.buffer = TargetBase;
				return STATUS_SUCCESS;
			}
			case CommType::cr3:
			{
				printf("Cr3 request\n");

				uint64 TargetBase = reinterpret_cast<uint64>(PsGetProcessSectionBaseAddress(TargetProcess));
				if (!TargetBase)
				{
					printf("Failed to get base\n");
					return STATUS_UNSUCCESSFUL;
				}

				printf("Base: 0x%llx\n", TargetBase);

				uint64 TargetCr3 = physical::cr3::GetFromBase(TargetBase);
				if (!TargetCr3)
				{
					printf("Failed to get cr3\n");
					return STATUS_UNSUCCESSFUL;
				}

				printf("Cr3: 0x%llx\n", TargetCr3);

				Data.buffer = TargetCr3;
				return STATUS_SUCCESS;
			}
			case CommType::read:
			{
				printf("Read request\n");

				size_t Bytes = 0;
				NTSTATUS Status = physical::ReadMemory(
					physical::cr3::StoredCr3,
					reinterpret_cast<void*>(Data.address),
					reinterpret_cast<void*>(Data.buffer),
					Data.size,
					&Bytes
				);
				if (!NT_SUCCESS(Status))
				{
					printf("Failed to read memory\n");
					return STATUS_UNSUCCESSFUL;
				}

				printf("Read %d bytes\n", Bytes);
				return STATUS_SUCCESS;
			}
			case CommType::write:
			{
				printf("Write request\n");

				size_t Bytes = 0;
				NTSTATUS Status = physical::WriteMemory(
					physical::cr3::StoredCr3,
					reinterpret_cast<void*>(Data.address),
					reinterpret_cast<void*>(Data.buffer),
					Data.size,
					&Bytes
				);
				if (!NT_SUCCESS(Status))
				{
					printf("Failed to write memory\n");
					return STATUS_UNSUCCESSFUL;
				}

				printf("Wrote %d bytes\n", Bytes);
				return STATUS_SUCCESS;
			}
			default:
			{
				return STATUS_NOT_IMPLEMENTED;
			}
		}
	}
}