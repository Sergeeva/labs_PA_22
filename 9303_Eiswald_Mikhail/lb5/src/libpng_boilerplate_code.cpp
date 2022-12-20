#include "libpng_boilerplate_code.h"

int Image::write(const char* filename) {
    FILE* f = fopen(filename, "wb");
    if(!f){
        return IO_ERROR;
    }

    this->write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!(this->write_ptr)){
        fclose(f);
        return IO_ERROR;
    }

    this->write_info = png_create_info_struct(this->write_ptr);
    if(!(this->write_info)){
        fclose(f);
        return IO_ERROR;
    }

    if (setjmp(png_jmpbuf(this->write_ptr))){
            fclose(f);
            return IO_ERROR;
    }

    png_init_io(this->write_ptr, f);

    if (setjmp(png_jmpbuf(this->write_ptr))){
          fclose(f);
          return IO_ERROR;
    }

    png_set_IHDR(write_ptr, write_info, width, height,
                 bit_depth, color_type,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE,
                 PNG_FILTER_TYPE_BASE);

    png_write_info(this->write_ptr, this->write_info);


    if (setjmp(png_jmpbuf(this->write_ptr))){
            fclose(f);
            return IO_ERROR;
    }

    png_write_image(this->write_ptr, this->row_pointers);

    if (setjmp(png_jmpbuf(this->write_ptr))){
          fclose(f);
          return IO_ERROR;
    }

    png_write_end(this->write_ptr, NULL);

    fclose(f);
    return 0;
}
