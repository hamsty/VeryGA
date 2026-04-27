import { useState } from 'react'
import { ReactFlow, Background, Controls } from '@xyflow/react';
import '@xyflow/react/dist/style.css';

const initialNodes = [
  {
    id: 'n1',
    position: { x: 0, y: 0 },
    data: { label: 'Node 1' },
    type: 'input',
  },
  {
    id: 'n2',
    position: { x: 100, y: 100 },
    data: { label: 'Node 2' },
  },
];

function App() {
  return (
    <div style={{ height: '800px', width: '600px' }}>
      <ReactFlow nodes={initialNodes}>
        <Background />
        <Controls />
      </ReactFlow>
    </div>
  );
}

export default App
