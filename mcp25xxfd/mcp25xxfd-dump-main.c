#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <linux/kernel.h>

#include "mcp25xxfd.h"

struct mcp25xxfd_mem {
	char buf[0x1000];
};

static int
mcp25xxfd_read_registers(struct mcp25xxfd_priv *priv,
			 struct mcp25xxfd_mem *mem,
			 const char *reg_file_path)
{
	FILE *reg_file;
	uint16_t reg;
	uint32_t val;

	reg_file = fopen(reg_file_path, "r");
	if (!reg_file)
		return -errno;

	while (fscanf(reg_file, "%hx: %x\n", &reg, &val) != EOF) {
		if (reg >= ARRAY_SIZE(mem->buf))
			return -EINVAL;

		*(uint32_t *)(mem->buf + reg) = val;
	}

	fclose(reg_file);

	return 0;
}

int regmap_bulk_read(struct regmap *map, unsigned int reg,
		     void *val, size_t val_count)
{
	memcpy(val, map->mem->buf + reg,
	       val_count * sizeof(uint32_t));

	return 0;
}

int main(int argc, char *argv[])
{
	struct mcp25xxfd_mem mem = { };
	struct regmap map = {
		.mem = &mem,
	};
	struct mcp25xxfd_priv priv = {
		.map = &map,
	};

	mcp25xxfd_read_registers(&priv, &mem, argv[1]);
	mcp25xxfd_dump(&priv);

	exit(EXIT_SUCCESS);
}
