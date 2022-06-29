/**
 * @file main.cpp
 * @author axelccccc (github.com/axelccccc)
 * @brief ASCII JPEG image viewer
 * @version 0.1
 * @date 2022-04-05
 * 
 * @copyright Copyright (c) axelccccc 2022
 * 
 * @todo Mean value of pixels for scaling ?
 * 
 */

#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>

#include <cstdlib>
#include <unistd.h>
#include <sys/ioctl.h>

#include <jpeglib.h>

const std::string graphics_chars = "Ñ#@W$9876543210?!abc;:+=-,._";



struct WindowDimensions {
    int char_rows;
    int char_columns;
};



WindowDimensions get_window_dimensions() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return { w.ws_row, w.ws_col };
}



int main(int argc, char **argv) {

    // check for filename arg
    if(argc < 2) {
        std::cout << "Usage: " << argv[0] << " <filename.jpg/jpeg>\n";
        exit(1);
    }

    // print libjpeg-turbo library version
    // std::cout << "libjpeg-turbo version: " << LIBJPEG_TURBO_VERSION_NUMBER << "\n";

    const std::string filename = argv[1];
    auto [win_height, win_width] = get_window_dimensions();

    // print window dimensions
    // std::cout << "Window dimensions: " << win_height << "x" << win_width << "\n";

    // libjpeg image read structures
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    // set up the error handler
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

    // open the file
    FILE *infile = fopen(filename.c_str(), "rb");
    if(!infile) {
        std::cout << "Could not open file: " << filename << "\n";
        exit(1);
    }
    jpeg_stdio_src(&cinfo, infile);

    // read the file header
    jpeg_read_header(&cinfo, TRUE);

    // print image dimensions
    std::cout << "Image dimensions: " << cinfo.image_width << "x" << cinfo.image_height << "\n";
    // print image colorspace
    switch(cinfo.out_color_space) {
        case JCS_GRAYSCALE:
            std::cout << "Image colorspace: grayscale\n";
            break;
        case JCS_RGB:
            std::cout << "Image colorspace: RGB\n";
            break;
        case JCS_YCbCr:
            std::cout << "Image colorspace: YCbCr\n";
            break;
        case JCS_CMYK:
            std::cout << "Image colorspace: CMYK\n";
            break;
        case JCS_YCCK:
            std::cout << "Image colorspace: YCCK\n";
            break;
        default:
            std::cout << "Image colorspace: unknown\n";
            break;
    }

    // set image output to grayscale
    cinfo.out_color_space = JCS_GRAYSCALE;

    // // scale image to window size
    // cinfo.scale_num = win_width;
    // cinfo.scale_denom = cinfo.image_width;

    // scale image to nearest ratio to smallest window dimension
    cinfo.scale_num = win_width < win_height ? win_width : win_height;
    cinfo.scale_denom = win_width < win_height ? cinfo.image_width : cinfo.image_height;

    // make output parameters valid
    jpeg_calc_output_dimensions(&cinfo);
    
    

    // start decompression
    jpeg_start_decompress(&cinfo);

    // print scaled image dimensions
    std::cout << "Scaled image dimensions: " << cinfo.output_width << "x" << cinfo.output_height << "\n";

    // make image buffer according to scaled dimensions
    std::vector<std::vector<unsigned char>> image_data(cinfo.output_height, std::vector<unsigned char>(cinfo.output_width, ' '));

    // allocate memory to store one scanline
    JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, 
                                                    JPOOL_IMAGE, 
                                                    cinfo.output_width * cinfo.output_components, 
                                                    1);

    // iterate over scanlines
    while(cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        for(int i = 0; i < win_width; i++) {
            image_data[cinfo.output_scanline - 1][i] = buffer[0][i];
        }
    }

    /* // print first row of image data values
    // std::copy(image_data[0].begin(), image_data[0].end(), std::ostream_iterator<unsigned char>(std::cout, " "));
    for(unsigned char c : image_data[12]) {
        std::cout << (int)c << " ";
    }
    std::cout << '\n'; */

    // print image data
    for(int i = 0; i < image_data.size(); i++) {
        for(int j = 0; j < image_data[0].size(); j++) {
            int char_index = (float)image_data[i][j] / (float)256 * (float)graphics_chars.size();
            std::cout << std::noskipws << 
                graphics_chars[graphics_chars.size() - char_index - 1] <<
                graphics_chars[graphics_chars.size() - char_index - 1];
        }
        std::cout << '\n';
    }

    // finish decompression
    jpeg_finish_decompress(&cinfo);

    // close image file
    fclose(infile);

    // destroy JPEG decompression object
    jpeg_destroy_decompress(&cinfo);

    /* // print image data
    for(const auto& row : image_data) {
        std::copy(row.begin(), row.end(), std::ostream_iterator<char>(std::cout));
        std::cout << '\n';
    } */
    
}