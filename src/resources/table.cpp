#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "table.h"

table::table()
{
	num_entries = 0;
	entries = 0;
}

table::~table()
{
	if (entries != 0)
	{
		for (int i = 0; i < num_entries; i++)
		{
			if (entries[i] != 0)
			{
				delete [] entries[i];
				entries[i] = 0;
			}
		}
		delete [] entries;
		entries = 0;
	}
}

int table::get_num_entries()
{
	return num_entries;
}

//constant because tables will not be modified by the [] operator
const char *table::operator[](int i)
{
	if (i < num_entries)
	{
		return entries[i];
	}
	else
	{
		return (char*)0;
	}
}

int table::compare(const void *a, const void *b)
{
	const char **aa = (const char**)a;
	const char **ba = (const char**)b;
	return strcmp(*aa, *ba);
}

void table::sort()
{
	//acp[4] is a function pointer to the compare function
	qsort(entries, num_entries, sizeof(char*), table::compare);
}

void table::print()
{
	printf("There are %d entries to print in %s\n", num_entries, table_name);
	for (int i = 0; i < num_entries; i++)
	{
		if (entries[i] != 0)
		{
			printf("Entry %d:\n\t%s \n", i, entries[i]);
			char moron;
			scanf("%c", &moron);
			if (moron == 'q')
				break;
		}
	}
	printf("\n");
}

void table::load(const char *real_name, files *place, file_locations location)
{
	char *buffer;
	int size;
	buffer = (char*)place->load_file(real_name, &size, location, 1);

	char* temp_entry;
	char delimiters[] = {0x0d, 0x0a, 0};
	temp_entry = strtok(buffer, delimiters);
	num_entries = atoi(temp_entry);
	if (num_entries == 0)
	{
		entries = 0;
		delete [] buffer;
		buffer = 0;
		return;
	}
	entries = new char*[num_entries];
	
	for (int i = 0; i < num_entries; i++)
	{
		entries[i] = (char*)0;
	}
	int i = 0;
	
	temp_entry = strtok(NULL, delimiters);
	while (temp_entry != 0)
	{
		if (i < num_entries)
		{
			entries[i] = new char[strlen(temp_entry) + 1];
			strcpy(entries[i], temp_entry);
			i++;
		}
		temp_entry = strtok(NULL, delimiters);
	}
	if (i < num_entries)
	{
//		printf("WARNING: %s only had (%d of %d) entries\n", real_name, i-1, num_entries);
		while (i < num_entries)
		{
			entries[i++] = (char*)0;
		}
	}

	delete [] buffer;
	buffer = 0;
}

void table::load_local(const char *name, files *place, file_locations location)
{
	char *real_name;
	real_name = new char[strlen(name) + 7];
	sprintf(real_name, "%s-%c.tbl", name, get_lang_char());
	load(real_name, place, location);
	delete [] real_name;
	real_name = 0;
}

char table::get_lang_char()
{
//	printf("STUB Get Language Character\n");
	return 'e';
}