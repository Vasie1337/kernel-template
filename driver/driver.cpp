#include <include.hpp>

EXTERN_C NTSTATUS Entry(void* param1, void* param2)
{
	const auto module = modules::get_kernel_module("ntoskrnl.exe");
	if (!module)
	{
		printf("Failed to get kernel module\n");
		return STATUS_UNSUCCESSFUL;
	}

	const auto text_section = modules::get_section(module, ".text");
	if (!text_section)
	{
		printf("Failed to get text section\n");
		return STATUS_UNSUCCESSFUL;
	}

	const auto cr3 = physical::cr3::GetFromBase(module.base);
	if (!cr3)
	{
		printf("Failed to get cr3\n");
		return STATUS_UNSUCCESSFUL;
	}

	printf("cr3: 0x%llx\n", cr3);
	
	uint64 buffer = 0;
	size_t bytes = 0;

	const auto status = physical::ReadMemory(cr3, reinterpret_cast<void*>(text_section.base), &buffer, sizeof(buffer), &bytes);
	if (!NT_SUCCESS(status))
	{
		printf("Failed to read memory\n");
		return STATUS_UNSUCCESSFUL;
	}

	printf("Read %d bytes\n", bytes);
	printf("Buffer: 0x%llx\n", buffer);

	return STATUS_SUCCESS;
}
