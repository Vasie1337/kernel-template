#include <include.hpp>

EXTERN_C NTSTATUS Entry(void* param1, void* param2)
{
	PEPROCESS process = 0;
	if (PsLookupProcessByProcessId((HANDLE)5672, &process) != STATUS_SUCCESS) {
		return STATUS_UNSUCCESSFUL;
	}

	printf("Process: 0x%llx\n", process);

	uintptr_t base = (uintptr_t)PsGetProcessSectionBaseAddress(process);
	if (!base)
	{
		printf("Failed to get base\n");
		return STATUS_UNSUCCESSFUL;
	}

	printf("Base: 0x%llx\n", base);

	const auto cr3 = physical::cr3::GetFromBase(base);
	if (!cr3)
	{
		printf("Failed to get cr3\n");
		return STATUS_UNSUCCESSFUL;
	}

	printf("cr3: 0x%llx\n", cr3);
	
	short buffer = 0;
	size_t bytes = 0;

	const auto status = physical::ReadMemory(cr3, reinterpret_cast<void*>(base), &buffer, sizeof(buffer), &bytes);
	if (!NT_SUCCESS(status))
	{
		printf("Failed to read memory\n");
		return STATUS_UNSUCCESSFUL;
	}

	printf("Read %d bytes\n", bytes);
	printf("Buffer: %lx\n", buffer);

	return STATUS_SUCCESS;
}
