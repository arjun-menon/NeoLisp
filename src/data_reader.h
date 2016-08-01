/**
 * Reads in a file containing lines of multiple numbers, e.g. a CSV of numbers.
 */

#ifndef VECTORCALC_DATA_READER_H
#define VECTORCALC_DATA_READER_H

/**
 * The consumer implements these callbacks.
 *
 *  startRow -- called before every row is processed
 *  addReal -- called for every number in the row
 *  endRow -- called after each row is processed
 */
class DataConsumer {
public:
    virtual void startRow() {};
    virtual void addReal(real val) = 0;
    virtual void endRow() {};
};

class DataReader {
    DataConsumer &dataConsumer;
    DataReader(DataConsumer &dataConsumer) : dataConsumer(dataConsumer) {}

    void process_file(const string &filename);
    void process_line(const string &line);

public:
    inline static void read_file(const string &filename, DataConsumer &dataConsumer) {
        DataReader(dataConsumer).process_file(filename);
    }
};

#endif //VECTORCALC_DATA_READER_H
