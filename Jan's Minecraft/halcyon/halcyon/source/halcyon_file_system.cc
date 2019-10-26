// halcyon_file_system.cc

#include "halcyon.h"
#include <stdio.h>

bool read_file_from_disk(const char *filename, file_content *content)
{
   if (!filename || !content)
      return false;

   FILE *fin = nullptr;
   fopen_s(&fin, filename, "r");
   if (!fin)
   {
      HC_ASSERT(false);
   }

   fseek(fin, 0, SEEK_END);
   size_t size = ftell(fin);
   fseek(fin, 0, SEEK_SET);

   uint8_t *data = new uint8_t[size + 1];
   size_t bytes = fread(data, 1, size, fin);
   fclose(fin);

   data[bytes] = '\0';

   content->size_ = size;
   content->data_ = data;

   return true;
}

bool read_file_from_disk_binary(const char *filename, file_content *content)
{
   if (!filename || !content)
      return false;

   FILE *fin = nullptr;
   fopen_s(&fin, filename, "rb");
   if (!fin)
   {
      HC_ASSERT(false);
   }

   fseek(fin, 0, SEEK_END);
   size_t size = ftell(fin);
   fseek(fin, 0, SEEK_SET);

   uint8_t *data = new uint8_t[size + 1];
   fread(data, 1, size, fin);
   fclose(fin);

   content->size_ = size;
   content->data_ = data;

   return true;
}

void release_file_content(file_content *content)
{
   if (!content)
      return;

   if (content->data_)
      delete [] content->data_;

   content->size_ = 0;
   content->data_ = nullptr;
}
