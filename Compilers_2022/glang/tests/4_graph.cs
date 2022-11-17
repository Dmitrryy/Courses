// compile:
// glang 10_fract.cs
// clang++ dump.ll ../../glib/SEngine.cpp -lsfml-graphics -lsfml-window -lsfml-system

class Program {
    // object with methods to manage window
    m_graph: Graphics;

    m_x: Integer;
    m_y: Integer;

    method Program() {
        // open window
        m_graph.Init(1000, 800);

        m_x.Set(0);
        m_y.Set(0);
        // loop while window is open
        while(m_graph.WindowIsOpen()) {
            // paint 500 pixels along x axis
            if(m_x.GreaterEqual(500)) {
                m_x.Set(0);
                m_y.Set(m_y.Add(1));
            }
            // red color
            m_graph.PutPixel(m_x, m_y, 4278190335);

            m_x.Set(m_x.Add(1));

            // display result
            m_graph.Flush();
        }

    }
}
