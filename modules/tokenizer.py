import numpy as np
from sklearn.datasets import fetch_20newsgroups
from sklearn.feature_extraction.text import CountVectorizer


newsgroups = fetch_20newsgroups(
    subset="train",
    remove=("headers", "footers", "quotes")
)

tf_vectorizer = CountVectorizer(max_df=0.9, min_df=2, max_features=1000,
                                    stop_words="english", dtype=np.int32)

datas = tf_vectorizer.fit_transform(newsgroups.data)

tf_datas = datas.T.toarray()
vocabulary = tf_vectorizer.vocabulary_

print("Shape : ", tf_datas.shape)
print("Vocabulary : ", tf_vectorizer.vocabulary_)


with open("input.npy", "wb") as f:
    np.save(f, tf_datas)

with open('labels.txt', 'w') as file:
    for label in vocabulary:
        file.write(label + '\n')