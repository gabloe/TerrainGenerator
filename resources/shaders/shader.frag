#version 150
out vec4 colorOut;
in vec4 v_Position;
void main(void)
{
    float C = 1 + v_Position.y;
    float R = 0.7 * C;
    float G = 0.8 * C;
    float B = 0.9 * C;
    colorOut= vec4( R , G , B , 1.0);
}
