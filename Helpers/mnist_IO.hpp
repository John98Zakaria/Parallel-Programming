#ifndef MNIST_IO_HPP
#define MNIST_IO_HPP

#include <cassert>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>

namespace helpers {

    std::vector<uint8_t> load_mnist_images(std::string filename)
    {
        std::ifstream ifile(filename, std::ios::binary);

        if(!ifile.good()) {
            throw std::ios_base::failure{"Can't read file " + filename};
        }

        uint32_t magic_number = 0; // 0x00000803(2051) -> datatype = uint8_t, dimensions = 3
        uint32_t num_images = 0;   // 60000 for train, 10000 for test
        uint32_t rows = 0;         // 28
        uint32_t cols = 0;         // 28

        // read and change byte order

        ifile.read(reinterpret_cast<char*>(&magic_number), sizeof(magic_number));
        magic_number = (magic_number >> 16) | (magic_number << 16);
        magic_number = ((magic_number & 0xFF00FF00) >> 8) | ((magic_number & 0xFF00FF) << 8);

        ifile.read(reinterpret_cast<char*>(&num_images), sizeof(num_images));
        num_images = (num_images >> 16) | (num_images << 16);
        num_images = ((num_images & 0xFF00FF00) >> 8) | ((num_images & 0xFF00FF) << 8);

        ifile.read(reinterpret_cast<char*>(&rows), sizeof(rows));
        rows = (rows >> 16) | (rows << 16);
        rows = ((rows & 0xFF00FF00) >> 8) | ((rows & 0xFF00FF) << 8);

        ifile.read(reinterpret_cast<char*>(&cols), sizeof(cols));
        cols = (cols >> 16) | (cols << 16);
        cols = ((cols & 0xFF00FF00) >> 8) | ((cols & 0xFF00FF) << 8);

        assert(magic_number == 0x00000803);
        assert(rows == 28);
        assert(cols == 28);

        size_t size = num_images*rows*cols;

        std::vector<uint8_t> images(size);

        ifile.read(reinterpret_cast<char*>(images.data()), size);
        ifile.close();

        return images;
    }

    std::vector<uint8_t> load_mnist_labels(std::string filename)
    {
        std::ifstream ifile(filename, std::ios::binary);

        if(!ifile.good()) {
            throw std::ios_base::failure{"Can't read file " + filename};
        }

        uint32_t magic_number = 0; // 0x00000801(2049) -> datatype = uint8_t, dimensions = 1
        uint32_t num_items = 0;    // 60000 for train, 10000 for test

        // read and change byte order

        ifile.read(reinterpret_cast<char*>(&magic_number), sizeof(magic_number));
        magic_number = (magic_number >> 16) | (magic_number << 16);
        magic_number = ((magic_number & 0xFF00FF00) >> 8) | ((magic_number & 0xFF00FF) << 8);

        ifile.read(reinterpret_cast<char*>(&num_items), sizeof(num_items));
        num_items = (num_items >> 16) | (num_items << 16);
        num_items = ((num_items & 0xFF00FF00) >> 8) | ((num_items & 0xFF00FF) << 8);

        assert(magic_number == 0x00000801);

        size_t size = num_items;

        std::vector<uint8_t> labels(size);

        ifile.read(reinterpret_cast<char*>(labels.data()), size);
        ifile.close();

        return labels;
    }

}

#endif
