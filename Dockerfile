FROM python:3.11-slim

LABEL maintainer="Max Chesterfield <chestm007@hotmail.com>"
LABEL description="Linux driver and daemon for Thermaltake RGB hardware"
LABEL version="1.3.0"

# Install system dependencies
RUN apt-get update && apt-get install -y \
    gcc \
    libusb-1.0-0-dev \
    libudev-dev \
    && rm -rf /var/lib/apt/lists/*

# Create non-root user
RUN useradd -m -u 1000 thermaltake

# Set working directory
WORKDIR /app

# Copy requirements and install Python dependencies
COPY setup.py VERSION ./
RUN pip install --no-cache-dir -e .

# Copy configuration
COPY linux_thermaltake_rgb/assets/config.yml /etc/linux_thermaltake_rgb/
RUN mkdir -p /etc/linux_thermaltake_rgb

# Create entrypoint script
RUN echo '#!/bin/bash\n\
if [ -f /config/config.yml ]; then\n\
    export THERMALTAKE_CONFIG_DIR=/config\n\
fi\n\
exec linux-thermaltake-rgb' > /entrypoint.sh && \
chmod +x /entrypoint.sh

# Expose volume for configuration
VOLUME ["/config"]

# Switch to non-root user (note: USB access requires running as root in Docker)
USER root

ENTRYPOINT ["/entrypoint.sh"]
