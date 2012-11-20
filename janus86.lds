OUTPUT_FORMAT("binary")
ENTRY(start)
SECTIONS
{
	.text 0x10000:
	{
		*(.text)
	}
	.data :
	{
		*(.data)
	}
	.bss :
	{
		*(.bss)
	}
}

