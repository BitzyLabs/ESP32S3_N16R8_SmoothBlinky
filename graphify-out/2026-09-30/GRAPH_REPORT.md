# Graph Report - ESP32S3_N16R8_SmoothBlinky  (2026-09-30)

## Corpus Check
- 17 files · ~13,632 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 6 file(s) not represented in the graph (top: (none) 5, .ini 1)

## Summary
- 93 nodes · 121 edges · 9 communities (8 shown, 1 thin omitted)
- Extraction: 90% EXTRACTED · 10% INFERRED · 0% AMBIGUOUS · INFERRED: 12 edges (avg confidence: 0.84)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `23a31f0a`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- Update & Hook Workflows
- Core Extraction Pipeline
- LED Rainbow Animation
- Graph Query Commands
- main.cpp Firmware Code
- Extraction Rules & Audit
- Extra Exports and Benchmark Reference
- uart_bridge_info.py

## God Nodes (most connected - your core abstractions)
1. `Query, Path, Explain Reference` - 11 edges
2. `graphify Skill (/graphify)` - 11 edges
3. `Incremental --update Pipeline` - 7 edges
4. `Extra Exports and Benchmark Reference` - 7 edges
5. `Incremental Update and Cluster-Only Reference` - 6 edges
6. `Semantic LLM Extraction (Part B)` - 6 edges
7. `Graph Outputs (graph.html, graph.json, GRAPH_REPORT.md)` - 6 edges
8. `Smooth Rainbow + White LED Effect` - 5 edges
9. `Structural AST Extraction (Part A)` - 5 edges
10. `Graphify Build Pipeline (Steps 0-9)` - 5 edges

## Surprising Connections (you probably didn't know these)
- `Native CLAUDE.md Integration` --semantically_similar_to--> `AGENTS.md graphify Rules`  [INFERRED] [semantically similar]
  .opencode/skills/graphify/references/hooks.md → AGENTS.md
- `Query-First Codebase Question Rule` --conceptually_related_to--> `Graph Outputs (graph.html, graph.json, GRAPH_REPORT.md)`  [INFERRED]
  AGENTS.md → .opencode/skills/graphify/SKILL.md
- `AGENTS.md graphify Rules` --references--> `Query, Path, Explain Reference`  [EXTRACTED]
  AGENTS.md → .opencode/skills/graphify/references/query.md
- `AGENTS.md graphify Rules` --references--> `God Nodes (High-Degree Community Hubs)`  [EXTRACTED]
  AGENTS.md → .opencode/skills/graphify/SKILL.md
- `Post-Change Graph Refresh Rule` --references--> `Structural AST Extraction (Part A)`  [EXTRACTED]
  AGENTS.md → .opencode/skills/graphify/SKILL.md

## Import Cycles
- None detected.

## Hyperedges (group relationships)
- **Graphify Export Suite** — _opencode_skills_graphify_references_exports_wiki_export, _opencode_skills_graphify_references_exports_neo4j_export, _opencode_skills_graphify_references_exports_falkordb_export, _opencode_skills_graphify_references_exports_mcp_server, _opencode_skills_graphify_references_exports_vector_exports, _opencode_skills_graphify_references_exports_token_benchmark [EXTRACTED 1.00]
- **Graph Query, Traversal and Feedback Flow** — _opencode_skills_graphify_references_query_vocab_expansion, _opencode_skills_graphify_references_query_bfs_traversal, _opencode_skills_graphify_references_query_dfs_traversal, _opencode_skills_graphify_references_query_networkx_fallback, _opencode_skills_graphify_references_query_path_command, _opencode_skills_graphify_references_query_explain_command, _opencode_skills_graphify_references_query_save_result, _opencode_skills_graphify_references_query_reflect_lessons [EXTRACTED 1.00]
- **Step 3 Part B Semantic Extraction Pipeline** — _opencode_skills_graphify_skill_semantic_extraction, _opencode_skills_graphify_skill_subagent_chunk_dispatch, _opencode_skills_graphify_skill_semantic_cache, _opencode_skills_graphify_skill_gemini_backend, _opencode_skills_graphify_references_extraction_spec_reference [EXTRACTED 1.00]

## Communities (9 total, 1 thin omitted)

### Community 0 - "Update & Hook Workflows"
Cohesion: 0.22
Nodes (13): graphify add and --watch Reference, URL Ingest (graphify add), Watch Mode (--watch), Native CLAUDE.md Integration, Post-Commit Auto-Rebuild Hook, Commit Hook and CLAUDE.md Integration Reference, Replace-on-Re-extract Build Merge, Cluster-Only Refresh (+5 more)

### Community 1 - "Core Extraction Pipeline"
Cohesion: 0.24
Nodes (10): God-Node Domain Hint Prompt, Video/Audio Transcription Reference, Whisper Video/Audio Transcription, Structural AST Extraction (Part A), Graphify Build Pipeline (Steps 0-9), Gemini Semantic Extraction Backend, God Nodes (High-Degree Community Hubs), Semantic LLM Extraction (Part B) (+2 more)

### Community 2 - "LED Rainbow Animation"
Cohesion: 0.24
Nodes (10): Gamma Correction for Perceptual Brightness, Hardware Information Serial Printout, HSV to RGB Color Conversion, Loop Mechanics (hue and whitePhase increments), Adafruit NeoPixel Library, PlatformIO Configuration (esp32s3dev), Sine Wave White Blending, Smooth Rainbow + White LED Effect (+2 more)

### Community 3 - "Graph Query Commands"
Cohesion: 0.28
Nodes (9): BFS Graph Query Traversal, DFS Graph Query Traversal, graphify explain (Node Explanation), Inline NetworkX Traversal Fallback, graphify path (Shortest Path Query), Query, Path, Explain Reference, Reflect / LESSONS.md Work Memory, save-result Feedback Loop (+1 more)

### Community 4 - "main.cpp Firmware Code"
Cohesion: 0.21
Nodes (8): gamma8(), hsvToRgb(), hueName(), loop(), printBootBanner(), printHardwareInfo(), printLiveFeed(), setup()

### Community 5 - "Extraction Rules & Audit"
Cohesion: 0.36
Nodes (8): Discrete Confidence Score Rubric, Hyperedge Extraction Rule, Node ID Naming Convention, Extraction Subagent Prompt Spec (extraction-spec.md), Verbatim source_file Attribution Rule, EXTRACTED/INFERRED/AMBIGUOUS Audit Trail, graphify Skill (/graphify), Honesty Rules

### Community 6 - "Extra Exports and Benchmark Reference"
Cohesion: 0.17
Nodes (13): FalkorDB Export (--falkordb), MCP Graph Server (graphify.serve), Neo4j Export (--neo4j), Extra Exports and Benchmark Reference, Token Reduction Benchmark, SVG and GraphML Exports, Wiki Export (--wiki), GitHub Repo Clone (graphify clone) (+5 more)

### Community 7 - "uart_bridge_info.py"
Cohesion: 0.18
Nodes (3): bcd_version(), find_port(), usb_descriptor()

## Knowledge Gaps
- **20 isolated node(s):** `Gamma Correction for Perceptual Brightness`, `HSV to RGB Color Conversion`, `Adafruit NeoPixel Library`, `Sine Wave White Blending`, `Cluster-Only Refresh` (+15 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 36 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **1 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `graphify Skill (/graphify)` connect `Extraction Rules & Audit` to `Update & Hook Workflows`, `Core Extraction Pipeline`, `Graph Query Commands`, `Extra Exports and Benchmark Reference`?**
  _High betweenness centrality (0.190) - this node is a cross-community bridge._
- **Why does `Query, Path, Explain Reference` connect `Graph Query Commands` to `Update & Hook Workflows`, `Extraction Rules & Audit`, `Extra Exports and Benchmark Reference`?**
  _High betweenness centrality (0.097) - this node is a cross-community bridge._
- **Why does `Extra Exports and Benchmark Reference` connect `Extra Exports and Benchmark Reference` to `Extraction Rules & Audit`?**
  _High betweenness centrality (0.062) - this node is a cross-community bridge._
- **What connects `Gamma Correction for Perceptual Brightness`, `HSV to RGB Color Conversion`, `Adafruit NeoPixel Library` to the rest of the system?**
  _20 weakly-connected nodes found - possible documentation gaps or missing edges._