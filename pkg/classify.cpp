
#include "classify.hpp"
#include "ldaplusplus/LDA.hpp"
#include "ldaplusplus/LDABuilder.hpp"
#include "ldaplusplus/Parameters.hpp"
#include <ldaplusplus/events/ProgressEvents.hpp>
#include <iostream>

Classify::Classify(std::string p_data)
: _lda(nullptr)
{
    _X = Common::import(p_data);
}

ldaplusplus::LDA<double>& Classify::getLda()
{
    return *_lda;
}


void Classify::buildLDA()
{
    ldaplusplus::LDABuilder<double> lda_builder = ldaplusplus::LDABuilder<double>()
        .set_classic_e_step(
            10,   // expectation iterations
            1e-2, // expectation tolerance
            0.01, // percentage of documents to compute likelihood for
            42    // the randomness seed
        )
        .set_classic_m_step()
        .initialize_topics_seeded(
            _X,  // the documents to seed from
            10, // the number of topics
            42  // the randomness seed-
        )
        .set_iterations(2)
        .set_workers(5);

        _lda = std::make_unique<ldaplusplus::LDA<double>>(lda_builder); //std::move(lda);
}

void Classify::addListener()
{

    _lda->get_event_dispatcher()->add_listener(
        [this](std::shared_ptr<ldaplusplus::events::Event> ev) {
            double * likelihood = &this->_likelihood;
            int * count_likelihood = &this->_count_likelihood;
            int * count = &this->_count;

            // an expectation has finished for a document
            if (ev->id() == "ExpectationProgressEvent") {
                (*count)++; // seen another document
                if (*count % 128 == 0) {
                    std::cout << (*count) << std::endl;
                }

                // aggregate the likelihood if computed for this document
                auto expev =
                    std::static_pointer_cast<ldaplusplus::events::ExpectationProgressEvent<double> >(ev);
                if (expev->likelihood() < 0) {
                    (*likelihood) += expev->likelihood();
                    (*count_likelihood)++;
                }
            }

            // A whole pass from the corpus has finished print the approximate per
            // document likelihood and reset the counters
            else if (ev->id() == "EpochProgressEvent") {
                std::cout << "Per document likelihood ~= "
                          << (*likelihood) / (*count_likelihood) << std::endl;
                (*likelihood) = 0;
                (*count_likelihood) = 0;
                (*count) = 0;
            }
        }
    );

}
void Classify::train()
{
    _lda->fit(_X);
}

void Classify::save_model(std::string path)
{
    std::fstream output_file(path, std::ios::out | std::ios::binary);

    auto model =
        std::static_pointer_cast<ldaplusplus::parameters::ModelParameters<double>>(
            _lda->model_parameters()
        );

    // save matrices and vectors that can be loaded using numpy.load()
    output_file << ldaplusplus::numpy_format::NumpyOutput<double>(model->alpha);
    output_file << ldaplusplus::numpy_format::NumpyOutput<double>(model->beta);
}