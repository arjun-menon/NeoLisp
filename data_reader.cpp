
#include "stdinclude.h"
#include "data_reader.h"

void DataReader::process_file(const string &filename) {
    ifstream csv_file(filename);
    if(csv_file.fail()) {
        throw runtime_error(string("File \"") + filename + "\" does not exist.");
    }

    string line;
    while(getline(csv_file, line)) {
        dataConsumer.startRow();

        process_line(line);

        dataConsumer.endRow();
    }
}

void DataReader::process_line(const string &line) {
    const char* int_pattern = "-1234567890";

    size_t pos = 0;
    while(pos < line.size()) {
        pos = line.find_first_of(int_pattern, pos);
        if(pos == string::npos)
            break;

        string::size_type num_len;
        int val;
        try {
            val = stoi(line.c_str() + pos, &num_len);
        } catch (invalid_argument) {
            pos++;
            continue;
        }

        dataConsumer.addInt(val, num_len);

        pos += num_len;
    }
}
