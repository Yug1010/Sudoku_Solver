const express = require("express");
const { spawn } = require("child_process");
const path = require("path");

const app = express();
const PORT = 3000;

// Path to your compiled C++ binary
const BINARY = path.join(__dirname, "sudoku");

app.use(express.json());
app.use(express.static(path.join(__dirname, "public")));

// POST /solve
// Body: { grid: [81 numbers, 0 for empty] }
// Response: { solved: true, grid: [81 numbers] } or { solved: false }
app.post("/solve", (req, res) => {
    const { grid } = req.body;

    if (!grid || grid.length !== 81) {
        return res.status(400).json({ error: "Send exactly 81 numbers." });
    }

    const input = grid.join(" ") + "\n";
    const proc = spawn(BINARY);

    let output = "";
    let errOutput = "";

    proc.stdout.on("data", (data) => (output += data.toString()));
    proc.stderr.on("data", (data) => (errOutput += data.toString()));

    proc.on("close", (code) => {
        if (output.includes("No solution exists")) {
            return res.json({ solved: false });
        }

        // Parse the "Solved Sudoku:" block from display() output
        const solvedIndex = output.indexOf("Solved Sudoku:");
        if (solvedIndex === -1) {
            return res.status(500).json({ error: "Unexpected output from solver." });
        }

        const solvedBlock = output.slice(solvedIndex);
        // Extract all single digits (1-9) in order from the grid display
        const digits = (solvedBlock.match(/\b[1-9]\b/g) || []).map(Number);

        if (digits.length !== 81) {
            return res.status(500).json({ error: "Could not parse solver output." });
        }

        res.json({ solved: true, grid: digits });
    });

    proc.on("error", () => {
        res.status(500).json({
            error: `Could not run binary '${BINARY}'. Did you compile sudoku.cpp?`,
        });
    });

    proc.stdin.write(input);
    proc.stdin.end();
});

app.listen(PORT, () => {
    console.log(`Sudoku server running → http://localhost:${PORT}`);
});
