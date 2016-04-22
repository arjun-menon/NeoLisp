
#include "stdinclude.h"
#include "data_reader.h"

void DataReader::process_file(const string &filename) {
    ifstream csv_file(filename);
    if(csv_file.fail()) {
        throw runtime_error(string("File \"") + filename + "\" does not exist.");
    }

    string line;
    while(getline(csv_file, line)) {
        process_line(line);
    }
}

void DataReader::process_line(const string &line) {
    bool did_add_item = false;
    const char* num_start_chars = "-1234567890";

    size_t pos = 0;
    while(pos < line.size()) {
        pos = line.find_first_of(num_start_chars, pos);
        if(pos == string::npos)
            break;

        size_t num_len;
        real val;
        try {
            val = stof(line.c_str() + pos, &num_len);
        } catch (invalid_argument) {
            pos++;
            continue;
        }

        if (!did_add_item) {
            dataConsumer.startRow();
            did_add_item = true;
        }

        dataConsumer.addReal(val, num_len);

        pos += num_len;
    }

    if(did_add_item) {
        dataConsumer.endRow();
    }
}
