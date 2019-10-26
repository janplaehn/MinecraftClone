// halcyon_file_system.h

#ifndef HALCYON_FILE_SYSTEM_H_INCLUDED
#define HALCYON_FILE_SYSTEM_H_INCLUDED

// note: handle based, query for read operation completion

struct file_content
{
   uint64_t size_;
   uint8_t *data_;
};

bool read_file_from_disk(const char *filename, file_content *content);
bool read_file_from_disk_binary(const char *filename, file_content *content);
void release_file_content(file_content *content);

#endif // HALCYON_FILE_SYSTEM_H_INCLUDED
