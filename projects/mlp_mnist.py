#MNIST 2-Layer MLP
import numpy as np
import matplotlib.pyplot as plt
import time
from keras.datasets import mnist

start_time = time.time()

#Learning Rate
rate = 0.1
#Hidden Units
hidden_units = 100
#Momentum
momentum = 0.9

#Loading MNIST Data (X represents images, Y represents labels)
(train_images, train_labels), (test_images, test_labels) = mnist.load_data()

#Preprocessing (0, 1)
train_images = (train_images / 255.0)
test_images = (test_images / 255.0)
#Reshaping images
train_images = train_images.reshape(-1, 784)
test_images = test_images.reshape(-1, 784)

#Initializing weights and biases for layers input(in) -> hidden(hi) and hidden(hi) -> output(ou)
weight_in_hi = np.random.rand(hidden_units, 784) * 0.1 - 0.05
weight_hi_ou = np.random.rand(10, hidden_units) * 0.1 - 0.05
bias_hi = 1 * np.random.rand(hidden_units, 1) * 0.1 - 0.05
bias_ou = 1 * np.random.rand(10, 1) * 0.1 - 0.05


## Backpropagation Algorithm ##
#Sigmoid function
def sigmoid(z):
    return 1 / (1 + np.exp(-z))
#Sigmoid derivative function
def sigmoid_derivative(y):
    return y * (1 - y)
#Shuffling training data since we are sequentially updating
def shuffle(train_images, train_labels):
    idx = np.random.permutation(len(train_images))
    train_images = train_images[idx]
    train_labels = train_labels[idx]
    return train_images, train_labels
#Forward phase
def forward(input, weight_in_hi, weight_hi_ou, bias_hi, bias_ou):
    #Forward Phase (hj, ok respectively)
    hidden_act = sigmoid(np.matmul(weight_in_hi, input) + bias_hi)
    output_act = sigmoid(np.matmul(weight_hi_ou, hidden_act) + bias_ou)
    return hidden_act, output_act
#Training step (Calculate errors, Update weights)
def train(input, target, hidden_act, output_act, weight_in_hi, weight_hi_ou, bias_hi, bias_ou, prev_in_hi, prev_hi_ou):
    #Calculate Error (dk, dj respectively)
    output_error = sigmoid_derivative(output_act) * (target - output_act)
    hidden_error = sigmoid_derivative(hidden_act) * np.matmul(weight_hi_ou.T, output_error)
    #Momentum Calculations
    momentum_term_in_hi = momentum * prev_in_hi
    momentum_term_hi_ou = momentum * prev_hi_ou
    prev_in_hi = np.matmul(hidden_error, np.transpose(input))
    prev_hi_ou = np.matmul(output_error, np.transpose(hidden_act))
    #Updating weights and biases (currently missing momentum)
    weight_hi_ou += rate * np.matmul(output_error, np.transpose(hidden_act)) + momentum_term_hi_ou
    weight_in_hi += rate * np.matmul(hidden_error, np.transpose(input)) + momentum_term_in_hi
    bias_ou += rate * output_error
    bias_hi += rate * hidden_error

## Training & Testing ##
epochs = 50
train_accuracy = np.zeros(epochs)
train_total = int(len(train_images)/2)
test_accuracy = np.zeros(epochs)
test_total = len(test_images)
#Momentum previous changes in weight
prev_in_hi = 0
prev_hi_ou = 0
for i in range(epochs):
    #Training
    train_images, train_labels = shuffle(train_images, train_labels)
    correct = 0
    for j in range (train_total):
        train_image = train_images[j].reshape((784, 1))
        target = np.full((10, 1), 0.1)
        target[train_labels[j]] = 0.9
        #Forward Phase (hj, ok respectively)
        hidden_act, output_act = forward(train_image, weight_in_hi, weight_hi_ou, bias_hi, bias_ou)
        train(train_image, target, hidden_act, output_act, weight_in_hi, weight_hi_ou, bias_hi, bias_ou, prev_in_hi, prev_hi_ou)
        prediction = np.argmax(output_act)
        if prediction == train_labels[j]:
            correct += 1
    train_accuracy[i] = (correct/train_total) * 100
    #Testing
    correct = 0
    for k in range(test_total):
        test_image = test_images[k].reshape((784, 1))
        #Forward Phase (hj, ok respectively)
        hidden_act, output_act = forward(test_image, weight_in_hi, weight_hi_ou, bias_hi, bias_ou)
        prediction = np.argmax(output_act)
        if prediction == test_labels[k]:
            correct += 1
    test_accuracy[i] = (correct/test_total) * 100
    print("Epoch {} finished..".format(i + 1))

#Graph
plt.plot(test_accuracy, label="Accuracy on the test data")
plt.plot(train_accuracy, label="Accuracy on the train data")
plt.xlabel("Epoch")
plt.ylabel("Accuracy (%)")
plt.xlim(0, epochs) #Graph was pushed to right for some reason, this fixed it
plt.legend(loc="lower right")
plt.title("Hidden Units = {}".format(hidden_units) + "  Momentum = {}".format(momentum))
plt.savefig("figure.png")
plt.show()

#Confusion Matrix
confusion = np.zeros((10, 10), dtype=int)  
for i in range(len(test_images)):
    test_image = test_images[i].reshape((784, 1))
    true_label = test_labels[i]
    unused, output_act = forward(test_image, weight_in_hi, weight_hi_ou, bias_hi, bias_ou)
    predicted_label = np.argmax(output_act)
    confusion[true_label][predicted_label] += 1
print("Test Confusion Matrix:")
print(confusion)

end_time = time.time()

print("Time: {}".format(end_time - start_time))