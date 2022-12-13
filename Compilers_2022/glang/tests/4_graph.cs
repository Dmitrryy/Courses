// compile:
// glang 4_graph.cs
// clang++ dump.ll ../../glib/SEngine.cpp -lsfml-graphics -lsfml-window -lsfml-system -O2

class Program {
    // object with methods to manage window
    m_graph: Graphics;

    m_x: Integer;
    m_y: Integer;

    // main
    method Program() {
        // open window
        m_graph.Init(1000, 800);

        m_x.Set(0);
        m_y.Set(0);

        // blue color
        color : Integer(0x0000FFFF);
        // loop while window is open
        while(m_graph.WindowIsOpen()) {
            m_graph.GetTime().Print();
            // paint 500 pixels along x axis
            if(m_x.GreaterEqual(500)) {
                m_x.Set(0);
                m_y.Set(m_y.Add(1));
            }
           
            m_graph.PutPixel(m_x, m_y, m_graph.GetRand());

            m_x.Set(m_x.Add(1));

            // display result
            m_graph.Flush();
        }

    }
}
