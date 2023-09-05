# Particle Systems

* Particule System
    - Particle
        -- Color
        - Velocity
        - Location
        - Behavior:
            1. Find particules in the neighborhood
            2. Compute cosine similarity between colors with neighbors
            3. if x < cosine similarity < y attraction else repulsion 