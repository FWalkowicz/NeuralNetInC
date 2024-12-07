## Neural Network Implementation in C

This repository contains a simple implementation of a feedforward neural network from scratch in C, created as a final project for a basic programming course at university.
The goal of this project is to demonstrate the fundamental concepts behind neural network training, including forward propagation, backpropagation, and gradient descent optimization.

## Features

- Feedforward Neural Network: Implements a basic 3-layer neural network with ReLU activation functions
- Backpropagation: Implements the backpropagation algorithm to train the neural network using gradient descent
- Gradient Calculation: Computes the gradients using mean squared error (MSE) loss function for regression tasks
- Training and Validation: Includes training and validation phases with performance metrics like MSE and R² score
- Custom Memory Management: All memory management (like matrix operations and weight updates) is handled manually in C.

## Known Issues

Exploding Gradients: In deeper networks, the gradients might explode, causing instability in training. This can be mitigated using techniques like Batch Normalization or gradient clipping (to be added in future improvements).

## Future Improvements

- Implement Batch Normalization to address exploding gradients
- Add support for more advanced activation functions like Leaky ReLU and ELU
- Implement additional optimizers like Adam for better convergence
- Refactor code for modularity and scalability.
