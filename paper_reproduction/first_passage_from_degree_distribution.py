file_name = input("alo, gimme the file name \n")

first_passage = [0]
max_value: int = 0

with open(file_name + ".txt") as f:
    lines = f.readlines()
    for line_number, line in enumerate(lines):
        values = line.split(" ")[:-1]
        print(values)
        for value in values:
            value = int(value)
            if value > max_value:
                max_value = value
                first_passage.append(line_number)

with open(file_name + "_first_passage.txt", 'w') as f:
    for line_number in first_passage:
        f.write(str(line_number) + "\n")