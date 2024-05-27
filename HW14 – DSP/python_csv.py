import csv
import matplotlib.pyplot as plt # for plotting
import numpy as np # for sine function

t = [] # column 0
data1 = [] # column 1

# reading csv files #1
with open('sigB.csv') as f:
    # open the csv file
    reader = csv.reader(f)
    for row in reader:
        # read the rows 1 one by one
        t.append(float(row[0])) # leftmost column
        data1.append(float(row[1])) # second column

# for i in range(len(t)):
    # # print the data to verify it was read
    # print(str(t[i]) + ", " + str(data1[i]))

# plotting code: #4

dt =  len(t)/t[-1] # samples per second
print(dt)
Ts = 1.0/dt; # sampling interval
y = data1 # the data to make the fft from
n = len(y) # length of the signal
k = np.arange(n)
T = n/dt
frq = k/T # two sides frequency range
frq = frq[range(int(n/2))] # one side frequency range
Y = np.fft.fft(y)/n # fft computing and normalization
Y = Y[range(int(n/2))]

# fig, (ax1, ax2) = plt.subplots(2, 1)
# ax1.plot(t,y,'b')
# ax1.set_xlabel('Time')
# ax1.set_ylabel('Amplitude')
# ax2.loglog(frq,abs(Y),'b') # plotting the fft
# ax2.set_xlabel('Freq (Hz)')
# ax2.set_ylabel('|Y(freq)|')

# plt.show()

# part 5
def moving_average_filter(data, X):
    filtered_data = []
    for i in range(len(data)):
        if i < len(data) - X + 1:
            filtered_data.append(np.mean(data[i:i+X]))
        else:
            ignore = 1
            filtered_data.append(np.mean(data[i:i+X - ignore]))
            ignore = ignore + 1
    return np.array(filtered_data)

# def plot_data_with_fft(original_data, filtered_data, X):
# def plot_data_with_fft(original_data, filtered_data, A, B):
def plot_data_with_fft(original_data, filtered_data, num_of_cof, filter_type, cutoff, bandwidth):
    # Plot original and filtered data
    plt.figure(figsize=(14, 6))

    plt.subplot(2, 1, 1)
    plt.plot(original_data, 'k-', label='Original Data')
    plt.plot(filtered_data, 'r-', label='Filtered Data')
    # plt.title(f'Original and Filtered Data (Averaged over {X} points)')
    plt.title(f'Original and Filtered Data with {cutoff}Hz Cutoff Frequency ({num_of_cof} Coefficients, {filter_type} Filter, {bandwidth}Hz Bandwidth)')
    plt.xlabel('Sample Number')
    plt.ylabel('Amplitude')
    plt.legend()

    # Compute FFT
    original_fft = np.fft.fft(original_data)
    original_fft = original_fft[range(int(n/2))]

    filtered_fft = np.fft.fft(filtered_data)
    filtered_fft = filtered_fft[range(int(n/2))]

    freqs = np.fft.fftfreq(len(original_data))
    freqs = freqs[range(int(n/2))]
        
    plt.subplot(2, 1, 2)
    plt.loglog(freqs, np.abs(original_fft), 'k-', label='Original FFT')
    plt.loglog(freqs, np.abs(filtered_fft), 'r-', label='Filtered FFT')
    plt.title('FFT of Original and Filtered Data')
    plt.xlabel('Frequency')
    plt.ylabel('Magnitude')
    plt.legend()

    plt.tight_layout()
    plt.show()


# X = 100
# filtered_data = moving_average_filter(data1, X)
# plot_data_with_fft(data1, filtered_data, X)

def IIF(data, A, B):
    new_average = []
    new_average.append(data[0])
    for i in range(1, len(data)):
        new_average.append(A * new_average[i-1] + B * data[i])
    return np.array(new_average)

# A = 0.95
# B = 0.05
# filtered_data = IIF(data1, A, B)
# plot_data_with_fft(data1, filtered_data, A, B)

def FIR(data, X_coefficients):
    X = len(X_coefficients)
    new_average = data[:X]
    # print(len(new_average))
    for i in range(X, len(data)):
        sum = 0
        for j in range(X):
            sum += X_coefficients[j] * data[i-j]  
        new_average.append(sum)
    return new_average

# # CASE A
# # Rectangular
# coefficients = []
# with open('case_a.csv') as f:
#     # open the csv file
#     reader = csv.reader(f)
#     for row in reader:
#         # read the rows 1 one by one
#         coefficients.append(float(row[0])) # leftmost column
# X = len(coefficients)
# num_of_cof = X
# sample_rate = 10000
# filter_type = "Rectangular"
# cutoff = 100
# bandwidth = 100

# CASE B
# Rectangular
coefficients = []
with open('case_b.csv') as f:
    # open the csv file
    reader = csv.reader(f)
    for row in reader:
        # read the rows 1 one by one
        coefficients.append(float(row[0])) # leftmost column
X = len(coefficients)
num_of_cof = X
filter_type = "Rectangular"
sample_rate = 3300
cutoff = 33
bandwidth = 33

# # CASE C
# # Rectangular
# coefficients = []
# with open('case_c.csv') as f:
#     # open the csv file
#     reader = csv.reader(f)
#     for row in reader:
#         # read the rows 1 one by one
#         coefficients.append(float(row[0])) # leftmost column
# X = len(coefficients)
# num_of_cof = X
# sample_rate = 2500
# filter_type = "Rectangular"
# cutoff = 25
# bandwidth = 25

# # CASE D
# # Rectangular
# coefficients = []
# with open('case_d.csv') as f:
#     # open the csv file
#     reader = csv.reader(f)
#     for row in reader:
#         # read the rows 1 one by one
#         coefficients.append(float(row[0])) # leftmost column
# X = len(coefficients)
# num_of_cof = X
# filter_type = "Rectangular"
# sample_rate = 400
# cutoff = 4
# bandwidth = 4

filtered_data = FIR(data1, coefficients)
plot_data_with_fft(data1, filtered_data, num_of_cof, filter_type, cutoff, bandwidth)


