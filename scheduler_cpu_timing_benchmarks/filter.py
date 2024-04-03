import csv
import argparse

def filter_and_write(input_file, output_file, filtervalue):
    with open(input_file, 'r', newline='') as input_csv, open(output_file, 'w', newline='') as output_csv:
        reader = csv.reader(input_csv, delimiter='\n')
        writer = csv.writer(output_csv, delimiter='\n')

        filtered_values = [value for row in reader for value in row if int(value) >= filtervalue]
        writer.writerows([[value] for value in filtered_values])

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Filter values greater than 7000 in a CSV file with newline separator")
    parser.add_argument("input_file", help="Input CSV file path")
    parser.add_argument("output_file", help="Output CSV file path")
    parser.add_argument("lowerbound", help="Values less than this value will be filtered out")
    args = parser.parse_args()

    filter_and_write(args.input_file, args.output_file, int(args.lowerbound))
