// Psychedelic fragment shader with retro 90s visual style
precision highp float;

// Uniforms passed from the application
uniform float time;       // Time in seconds
vec2 u_resolution = vec2(1920, 1080);

// Constants for the effect
const float SPEED = 1;
const float INTENSITY = 2;
const float COMPLEXITY = 2.0;

// Function to create a smooth color transition
vec3 colorPulse(float t, vec3 color1, vec3 color2, vec3 color3) {
    float p = fract(t);
    if (p < 0.33) {
        return mix(color1, color2, p * 3.0);
    } else if (p < 0.66) {
        return mix(color2, color3, (p - 0.33) * 3.0);
    } else {
        return mix(color3, color1, (p - 0.66) * 3.0);
    }
}

void main() {
    // Normalize coordinates to [-1, 1] range with aspect ratio correction
    vec2 uv = (gl_FragCoord.xy - 0.5 * u_resolution.xy) / min(u_resolution.x, u_resolution.y);
    
    // Calculate distance from center
    float dist = length(uv);
    
    // Calculate angle from center
    float angle = atan(uv.y, uv.x);
    
    // Create time-based variables for animation
    float t1 = time * SPEED;
    float t2 = time * SPEED * 0.7;
    
    // Create wave patterns
    float wave1 = sin(dist * 10.0 + t1) * 0.5 + 0.5;
    float wave2 = cos(angle * 8.0 + t2) * 0.5 + 0.5;
    float wave3 = sin(dist * 15.0 - angle * 6.0 + t1 * 1.3) * 0.5 + 0.5;
    float wave4 = cos(dist * 8.0 + angle * 4.0 - t2 * 1.1) * 0.5 + 0.5;
    
    // Create spiral effect
    float spiral = sin(dist * COMPLEXITY * 10.0 - angle * 10.0 + t1 * 2.0) * 0.5 + 0.5;
    
    // Combine waves with different weights
    float pattern = wave1 * 0.3 + wave2 * 0.2 + wave3 * 0.25 + wave4 * 0.25 + spiral * INTENSITY;
    
    // Create pulsating colors - bright neon colors for 90s retro feel
    vec3 color1 = vec3(1.0, 0.2, 0.8);  // Hot pink
    vec3 color2 = vec3(0.2, 0.8, 1.0);  // Cyan
    vec3 color3 = vec3(1.0, 0.9, 0.1);  // Yellow
    
    // Create secondary colors
    vec3 color4 = vec3(0.6, 0.0, 1.0);  // Purple
    vec3 color5 = vec3(0.0, 1.0, 0.5);  // Green
    
    // Create color pulses at different rates
    vec3 pulseA = colorPulse(t1 * 0.2, color1, color2, color3);
    vec3 pulseB = colorPulse(t1 * 0.1 + 0.3, color3, color4, color5);
    
    // Mix colors based on the pattern
    vec3 finalColor = mix(pulseA, pulseB, pattern);
    
    // Add some brightness variation
    finalColor *= 0.8 + 0.4 * sin(t1 + dist * 5.0);
    
    // Add subtle vignette effect for retro feel
    finalColor *= 1.0 - dist * 0.5;
    
    // Output the final color
    gl_FragColor = vec4(finalColor, 0.5);
}