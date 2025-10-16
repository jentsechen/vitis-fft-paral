import numpy as np
import matplotlib.pyplot as plt
import plotly.graph_objs as go
import plotly.offline as pof
from plotly.subplots import make_subplots

def print_info(data):
    print("Shape: ", data.shape)
    print("Size: ", data.size)
    print("Data type: ", data.dtype)
    print("Dimensions: ", data.ndim)

if __name__ == "__main__":
    input = np.load("data_rx_10_complex_64.npy")[0]
    print(len(input))
    input_zp = np.concatenate([input, np.zeros(2**20-len(input))])
    output = np.load("output.npy")
    print_info(output)
    print(len(output))

    fft_size = 2**10
    figure = make_subplots(rows=2, cols=1)
    start = 21
    result = output[start*fft_size:(start+1)*fft_size]
    golden = np.fft.fft(input_zp[start*fft_size:(start+1)*fft_size])
    figure.add_trace(go.Scatter(y=result.real), row=1, col=1)
    figure.add_trace(go.Scatter(y=golden.real), row=1, col=1)
    figure.add_trace(go.Scatter(y=result.imag), row=2, col=1)
    figure.add_trace(go.Scatter(y=golden.imag), row=2, col=1)
    figure.write_html("result.html")

    print("DONE")
