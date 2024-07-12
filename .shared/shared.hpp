#pragma once

enum class CommType : unsigned long
{
	test = 1,
	read = 2,
	write = 3,
	base = 4,
	cr3 = 5,
};

struct CommData 
{
	unsigned short magic;

	CommType type;

	unsigned long pid;

	unsigned long long address;
	unsigned long long buffer;
	unsigned long long size;

	volatile bool delivered;

	bool IsValid() const
	{
		return magic == 0x72FE;
	}
};