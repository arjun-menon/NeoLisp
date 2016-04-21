/**
 * Reads in a file containing lines of multiple integers, e.g. a CSV of integers.
 */

#ifndef VECTORCALC_DATA_READER_H
#define VECTORCALC_DATA_READER_H

/**
 * The consumer implements these callbacks.
 *
 *  startRow -- called before every row is processed
 *  addInt -- called for every integer in the row
 *  endRow -- called after each row is processed
 */
class DataConsumer {
public:
    virtual void startRow() {};
    virtual void addInt(int val, string::size_type len) = 0;
    virtual void endRow() {};
};

class DataReader {
private:
    void process_file(const string &filename);
    void process_line(const string &line);

    DataConsumer &dataConsumer;
    DataReader(DataConsumer &dataConsumer) : dataConsumer(dataConsumer) {}

public:
    inline static void read_file(const string &filename, DataConsumer &dataConsumer) {
        DataReader(dataConsumer).process_file(filename);
    }
};

#endif //VECTORCALC_DATA_READER_H
