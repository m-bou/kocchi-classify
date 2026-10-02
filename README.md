# kocchi-classify

> ⚠️ **Deprecated, not used in production.** This classifier is **not** what powers the current version of Kocchi. It is a historical project from 2023–2024, kept public only to show the approach I took back then (the crawl, the topic-modelling funnel and the engineering trade-offs). It is no longer maintained, the Instagram endpoints it depends on no longer work, and the current Kocchi uses a different, newer implementation.

A C++20 **Latent Dirichlet Allocation (LDA)** trainer used to classify Instagram profiles by topic. It is **stage 4** of the [Topicz](https://github.com/m-bou/topicz-scrapper) funnel (discover → crawl → heuristic gate → **classify** → shortlist) and is included in topicz-scrapper as a submodule at `pkg/kocchi-classify`.

- Project story and numbers: [topicz-scrapper README](https://github.com/m-bou/topicz-scrapper#readme)
- Full architecture: [topicz-scrapper `docs/ARCHITECTURE.md`](https://github.com/m-bou/topicz-scrapper/blob/main/docs/ARCHITECTURE.md)
- LDA engine: [kocchi-lda](https://github.com/m-bou/kocchi-lda) (a fork of [LDA++](https://github.com/angeloskath/supervised-lda))

> Status: **working prototype**. It runs end to end on test data; paths, topic count and iterations are hardcoded (see [Status](#status)).

## Role in Topicz

A crawl of Instagram profiles contains many off-target accounts (foreign fan pages, generic food accounts, spam). Even after a keyword/language gate, the remaining profiles must be sorted into business categories (for the Kocchi use case: Asian-event organizers, exhibitors/venues, influencers/media, other). kocchi-classify trains an unsupervised topic model on the profiles' text; the topics are then mapped to the categories. Classification is what turns a raw crawl into a usable shortlist.

## Why C++

- LDA++ provides **variational EM with multi-threaded E-steps** (`set_workers`) and Eigen vectorisation.
- It also provides supervised LDA variants (sLDA, fsLDA) for a later labelled approach.
- **No JVM**, unlike MALLET (used in the first generation of the notebooks).
- On roughly 1.3M posts, Python LDA was too slow on a laptop.

## Data contract

**Input**
- `*.npy`: a **vocabulary × documents** matrix of `int32` word counts (rows = words, columns = documents; this is the transpose of scikit-learn's usual layout).
- `labels.txt`: the vocabulary, one word per line.

**Output**: an `.npy` stream with **α** (vector, the Dirichlet prior over topics) and **β** (topics × vocabulary word distributions), written with `NumpyOutput<double>`, so Python can load it with `numpy.load`.

```python
# Produce the input
from sklearn.feature_extraction.text import CountVectorizer
import numpy as np

docs = ["sushi ramen tokyo", "kpop concert paris", ...]
vec = CountVectorizer(max_df=0.9, min_df=2, max_features=1000)
X = vec.fit_transform(docs).astype(np.int32)
np.save("input.npy", X.T.toarray())                      # vocabulary × documents
open("labels.txt", "w").write("\n".join(vec.get_feature_names_out()))

# Read the output (the file is a stream of arrays: alpha then beta)
with open("model.npy", "rb") as f:
    alpha = np.load(f)
    beta = np.load(f)
```

## How it works

`main.cpp` starts a `QCoreApplication`, builds a `Classify`, then calls `buildLDA()`, `addListener()`, `train()` and `save_model(...)`.

`pkg/classify.{hpp,cpp}`:
1. `Common::import` loads an `Eigen::MatrixXi` through `ldaplusplus::numpy_format::NumpyInput<int>`.
2. `buildLDA()` configures `LDABuilder<double>`:
   - `set_classic_e_step(10, 1e-2, 0.01, 42)`: up to 10 E-step iterations, convergence tolerance 1e-2, likelihood computed on 1% of the documents, seed 42.
   - `set_classic_m_step()`: the standard M-step.
   - `initialize_topics_seeded(X, 10, 42)`: **10 topics**, initialised from the data with seed 42 (deterministic runs).
   - `set_iterations(2)`: 2 EM iterations.
   - `set_workers(5)`: 5 threads for the E-step.
3. `addListener()` subscribes to the LDA++ event dispatcher: it counts `ExpectationProgressEvent` (printing every 128 documents) and prints the per-document log-likelihood on every `EpochProgressEvent`.
4. `save_model()` writes α and β as `.npy`.

Other files: `pkg/Common.{hpp,cpp}` (QString-to-stream helpers, `PRINT_DEBUG` / `PRINT_ERROR` macros, matrix printer), `pkg/Tokenizer.{hpp,cpp}` (C++ tokenizer stub using `std::format`, hence C++20), `pkg/profile.cpp` (stub).

## Build

Prerequisites: CMake ≥ 3.14, a C++20 compiler, Qt5 (Core, Widgets), Eigen ≥ 3.3, pybind11, LDA++ (provided by kocchi-lda), Python 3 with development headers:

```bash
sudo apt-get install python3-dev python3-distutils
cmake -B build && cmake --build build
```

CMake creates a **virtualenv in the build directory at configure time** (numpy, scipy, scikit-learn) and copies `modules/tokenizer.py` into the build tree. The binary lands in `build/bin/classify`.

## Testing with the 20newsgroups fixture

`modules/tokenizer.py` is only a **test fixture**; the real tokenizer is `pkg/common/pylib/tokenizer.py` in [topicz-scrapper](https://github.com/m-bou/topicz-scrapper), which also writes a `vocabulary.npy` (same words as `labels.txt`, unused by the C++ side). It builds a 1,000-feature `CountVectorizer` matrix from scikit-learn's 20newsgroups dataset, transposes it and writes `input.npy` + `labels.txt`. It was used to validate the C++ path before real data.

## Status

A **working prototype**: the pipeline runs end to end on test data. Honest limits:
- the input `.npy` path is hardcoded in `main.cpp` (a developer-local path);
- the number of topics (10) and iterations (2) are fixed;
- it was never wired to production data before the project stopped, so Topicz's real runs used the MALLET notebooks (generation 1).

History: 2024-03-23 init → CMake → pybind working (05-04) → document-term matrix generated (05-05) → model training (05-06) → tokenizer, C++20, debug macros (05-11) → venv fix (05-22).

## Layout

```
main.cpp              entry point
pkg/classify.*        Classify: import, build LDA, listeners, save
pkg/Common.*          helpers and debug macros
pkg/Tokenizer.*       tokenizer stub (C++20)
modules/tokenizer.py  20newsgroups test fixture
cmake/                Find modules (Docopt, Eigen3, LDA++, BashCompletion)
```

Related: [topicz-scrapper](https://github.com/m-bou/topicz-scrapper) · [kocchi-lda](https://github.com/m-bou/kocchi-lda).

## License

MIT, see [LICENSE](LICENSE).
