# Data-Transaction-Level-Abstraction

Digital Design that started in 1960’s at the transistor level, abstracted to gate-level and then to RTL in late 1980’s. By early 90’s a complete ecosystem that included HDLs, EDA tools, Data and Control Flow mechanisms, and RTL design methodology was established. The correspondence of this level of abstraction with the actual hardware, and the convenient hardware oriented ecosystem around it helped its acceptance by the digital design community.

We are now at a point that designs have become more complex and for this complexity, RTL requires too much details for a hardware designer to attend to. Thus a step above RTL is needed. We are taking this step for the specific domain of Neural Network hardware accelerators. An ecosystem for design of NN accelerators is being proposed here.

Learning from RTL, this NN design ecosystem consists of an HDL, design tools, data and control flow mechanisms, and a methodology of design. More importantly, this ecosystem has direct correspondance with RTL, and using its related tools, NN accelerator descriptions can automatically be synthesized to RTL.

This talk presents of our Neural Network Design ecosystem. The HDL used here is based on SystemC that automatically provides for its simulation. For synthesis we are proposing an abstract synthesis methodology that translates SystemC descriptions of  NN accelerators to their equivalent RTL descriptions. Abstract communications and data and control flow mechanisms in this echo system have direct correspondence to RTL. A NN accelerator example will be discussed and described in SystemC. We will show the use of abstract SystemC channels for communications between NN layers, and show SystemC event-based protocols for layer to layer communications.

