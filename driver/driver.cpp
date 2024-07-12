#include <include.hpp>

EXTERN_C NTSTATUS Entry(void* param1, void* param2)
{
	const auto module = modules::get_kernel_module("ntoskrnl.exe");
	if (!module)
	{
		printf("Failed to get kernel module\n");
		return STATUS_UNSUCCESSFUL;
	}

	const auto cr3 = physical::cr3::GetFromBase(module.base);
	if (!cr3)
	{
		printf("Failed to get cr3\n");
		return STATUS_UNSUCCESSFUL;
	}

	printf("cr3: 0x%llx\n", cr3);

	uint16 buffer = 0;
	size_t read = 0;
	NTSTATUS status = physical::ReadMemory(cr3, reinterpret_cast<void*>(module.base), &buffer, sizeof(buffer), &read);
	if (!NT_SUCCESS(status))
	{
		printf("Failed to read memory\n");
		return STATUS_UNSUCCESSFUL;
	}

	printf("First short: 0x%x\n", buffer);

	return STATUS_SUCCESS;
}
