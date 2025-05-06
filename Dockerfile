# Use an official Ubuntu base image
FROM ubuntu:20.04

# Set the working directory inside the container
WORKDIR /app

# Install required dependencies
RUN apt-get update && apt-get install -y \
    g++ \
    make \
    libcurl4-openssl-dev \
    && rm -rf /var/lib/apt/lists/*

# Copy the source code and Makefile into the container
COPY . .

# Build the application using the Makefile
RUN make

# Expose the port the application will run on
EXPOSE 8080

# Command to run the application
CMD ["./web-server"]