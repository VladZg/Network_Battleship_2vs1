def rotate_90_deg(old_str):
    old_str = old_str.replace(" ", "").replace("\n", "")
    new_str = ''
    for n, char in enumerate(old_str):
        m = 9 + 10*(n % 10) - (n // 10)
        new_str += old_str[m % len(old_str)]
    return new_str+"\n"

def rotate_180_deg(old_str):
    return rotate_90_deg(rotate_90_deg(old_str))

def rotate_270_deg(old_str):
    return rotate_90_deg(rotate_90_deg(rotate_90_deg(old_str)))

def horizontal_symmetry(old_str):
    old_str = old_str.replace(" ", "").replace("\n", "")
    new_str = ''
    for n, char in enumerate(old_str):
        m = 90 + n % 10 - 10 * (n // 10)
        new_str += old_str[m % len(old_str)]
    return new_str+"\n"

def verical_symmetry(old_str):
    return rotate_90_deg(rotate_90_deg(horizontal_symmetry(old_str)))

def transition_6(old_str):
    return rotate_90_deg(horizontal_symmetry(old_str))

def transition_7(old_str):
    return horizontal_symmetry(rotate_90_deg(old_str))

with open('initial_placements.txt', 'r') as input_file:
    with open('output_placements.txt', 'w') as output_file:
        for line in input_file:
            output_file.write(line.replace(" ", ""))
            output_file.write(rotate_90_deg(line))
            output_file.write(rotate_180_deg(line))
            output_file.write(rotate_270_deg(line))
            output_file.write(horizontal_symmetry(line))
            output_file.write(verical_symmetry(line))
            output_file.write(transition_6(line))
            output_file.write(transition_7(line))
