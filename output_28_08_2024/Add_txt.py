
# List of filenames
filenames = ['Efficiency_3500_keV_hist_det8.txt', 'Efficiency_3500_keV_hist_det9.txt', 'Efficiency_3500_keV_hist_det10.txt', 'Efficiency_3500_keV_hist_det11.txt']

# Initialize a dictionary to store the sum of values
data = {}

# Read data from each file and sum the values
for filename in filenames:
    with open(filename, 'r') as f:
        for line in f:
            parts = line.split()
            key = float(parts[0])
            value = int(parts[1])
            if key in data:
                data[key] += value
            else:
                data[key] = value

# Convert the dictionary to a sorted list of tuples
sorted_data = sorted(data.items())

# Write the combined result to a new file
with open('Efficiency_3500_keV_combined_det8.txt', 'w') as out_file:
    for key, value in sorted_data:
        out_file.write(f'{key}\t{value}\n')

print('Combined result written to Efficiency_3500_keV_combined_det8.txt')

