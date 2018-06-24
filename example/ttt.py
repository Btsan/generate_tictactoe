import numpy as np
from keras.models import Sequential
from keras.layers import Dense
from keras import regularizers
from keras.models import model_from_json

# # 0-8: board configuration, 9-10: possible lines [0, 8], 11-12: with aligned marks [0, 8], 13: winner [-1, 1]
data = np.zeros((5478, 14), dtype='b')

with open('ttt.txt', 'r') as ttt:
	for i, line in enumerate(ttt):
		line = line.strip()
		for j, sub in enumerate(line.split(' ')):
			data[i, j] = int(sub);

np.save('data.npy', data)
# # once this block of code is ran once, comment it all out

# load previously read game state data (includes board values and heuristic metrics)
data = np.load('data.npy')

gen_model = Sequential()

# generative model that only takes in metrics
gen_model.add(Dense(8, activation='relu', input_dim=5, kernel_regularizer=regularizers.l2(0.1)))
gen_model.add(Dense(1, activation='linear'))
gen_model.compile(optimizer='nadam', loss='mean_squared_error')

labels = np.zeros(5478, dtype='b')

# intialize weak labels, using from generated heuristics: delta(pieces) + 2*delta(lines) + 10*winner
labels += (data[:, 9] - data[:, 10]) + 2*(data[:, 11] - data[:, 12]) + 10*data[:, 13]

# take game state data and weak labels to train model
gen_model.fit(data[:, 9:], labels, epochs=64, batch_size=256, verbose=0)
print('gen total loss: ', gen_model.test_on_batch(data[:, 9:], labels)*5478)

# calculate probabilistic (still noisy) labels using trained generative model
probabilistic = gen_model.predict_on_batch(data[:, 9:])

# calculate confidence score for each label from the error between the original weak label and the new probabilistic label
confidence = 1/(1 + np.absolute(labels - probabilistic[:, 0]))

# discriminative model that learns from just the board values and probabilistic labels
disc_model = Sequential()

# use regularizers for noisy labels
disc_model.add(Dense(16, activation='linear', input_dim=9, kernel_regularizer=regularizers.l2(0.01)))
disc_model.add(Dense(8, activation='relu', kernel_regularizer=regularizers.l2(0.01)))
disc_model.add(Dense(4, activation='relu', kernel_regularizer=regularizers.l2(0.01)))
disc_model.add(Dense(1, activation='linear', kernel_regularizer=regularizers.l2(0.01)))
disc_model.compile(optimizer='nadam', loss='mean_squared_error')

# sample_weight=confidence weights the loss calculation and limits step
disc_model.fit(data[:, :9], probabilistic, epochs=128, batch_size=256, sample_weight=confidence, verbose=0)
print('disc total loss: ', disc_model.test_on_batch(data[:, :9], probabilistic)*5478)

# save model
disc_json = disc_model.to_json()
with open('dp.json', 'w') as json_file:
	json_file.write(disc_json)
disc_model.save_weights('dp.h5')