// compile:
// glang 999_fract.cs
// clang++ dump.ll ../../glib/SEngine.cpp -lsfml-graphics -lsfml-window -lsfml-system

class Program {
    // object with methods to manage window
    m_graph: Graphics;

    m_width: Integer;
    m_heigth: Integer;

    // methods
    //=-------

    method pack_4ui8_to_ui32(r: Integer, g: Integer, b: Integer, a: Integer) : Integer {
        result : Integer;
        result.Set(0);
        result = result | (a);
        result = result | (b << 8);
        result = result | (g << 16);
        result = result | (r << 24);
        return result;
    }

    method abs_dummy(x: Integer) : Integer {
        if(x < 0) {
            x = -x;
        }
        return x;
    }


    /// @brief Integer approximation of cos. Period T = 4000. 
    /// A parabola is used for approximation.
    /// @param x - arg
    /// @return int32_t - approximate value of cos * 10^3.
    method int_cos(x: Integer) : Integer {
        fracted : Integer;
        per_id : Integer;
        sign : Integer;
        y : Integer;

        fracted = x % 1000;

        per_id = (this.abs_dummy(x) % 4000) / 1000;
        if(per_id % 2) {
            fracted = 1000 - fracted;
        }
        
        sign = -1;
        if((per_id == 1) | (per_id == 2)) {
            sign = 1;
        }
        y = (fracted * fracted) / 1000 - 1000;
        return y * sign;
    }


    method palette_int(t: Integer, d1: Integer, d2: Integer, d3: Integer) : Integer {
        x : Integer;
        y : Integer;
        z : Integer;

        x = (500 + 500 * this.int_cos(628318 * (1000 * t / 1000 + d1) / 100000) / 1000) / 5;
        y = (500 + 500 * this.int_cos(628318 * (1000 * t / 1000 + d2) / 100000) / 1000) / 5;
        z = (500 + 500 * this.int_cos(628318 * (0    * t / 1000 + d3) / 100000) / 1000) / 5;

        return this.pack_4ui8_to_ui32(x, y, z, t / 4);
    }


    method shader(x: Integer, y: Integer, time: Integer) : Integer {
        result : Integer;
        uv_x : Integer;
        uv_y : Integer;
        r : Integer;
        c1 : Integer;
        c2 : Integer;
        z1 : Integer;
        z2 : Integer;
        maxIteration : Integer;
        iteration : Integer;

        result = 0x000000FF;

        uv_x = x * 1000 / m_width;
        uv_y = y * 1000 / m_heigth;
        
        maxIteration = 30;

        r = 788 * (this.int_cos((time / 3) - 2000) / 5 + 850) / 1000; //10^-3

        c1 = r * this.int_cos(time / 3) / 1000;
        c2 = r * this.int_cos(time / 3 + 3000) / 1000;

        z1 = 3 * (uv_x - 500);
        z2 = 2 * (uv_y - 500);

        iteration = 0;
        while(iteration < maxIteration) {
            z1_tmp : Integer;
            z1_tmp = (z1 * z1 - z2 * z2) / 1000 + c1;
            z2 = (z2 * z1 + z1 * z2) / 1000 + c2;
            z1 = z1_tmp;
            if((z1 * z1 + z2 * z2) / 10000 > 4000) {
                break;
            }

            iteration = iteration + 1;
        }

        if(iteration != maxIteration) {
            result = this.palette_int(iteration * 1000 / maxIteration,
                300 + (this.int_cos(time + 3000) * 2 / 5),
                200 + (this.int_cos(time + 3600) * 3 / 10),
                200 + (this.int_cos(time + 4400) * 3 / 10)
                );
        }
        return result;
    }


    // main
    method Program() {
        // open window
        m_width.Set(1024);
        m_heigth.Set(576);
        m_graph.Init(m_width, m_heigth);

        time : Integer;
        x : Integer;
        y : Integer;
        // loop while window is open
        while(m_graph.WindowIsOpen()) {
            time.Set(m_graph.GetTime() / 2);

            y = 0;
            while(y < m_heigth) {
                x = 0;
                while(x < m_width) {
                    m_graph.PutPixel(x, y, this.shader(x, y, time));
                    x = x.Add(1);
                }
                y = y.Add(1);
            }

            // display result
            m_graph.Flush();
        }

    }
} // class Program
