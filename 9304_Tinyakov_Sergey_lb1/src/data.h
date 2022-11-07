#ifndef DATA_H
#define DATA_H

struct ThreadReaderMatrix{
    unsigned char flag;
    char mode;
    int id;
    char *filename;
    int *data;
};

struct ThreadSummatorMatrix{
    long start_index, end_index;
    int *data1, *data2;
};

struct ThreadWriterMatrix{
    char mode;
    long height, width;
    char *filename;
    int *data;
};

typedef struct ThreadReaderMatrix ThreadReaderMatrix;

typedef struct ThreadSummatorMatrix ThreadSummatorMatrix;

typedef struct ThreadWriterMatrix ThreadWriterMatrix;

#endif // DATA_H
