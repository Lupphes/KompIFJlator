#ifndef DATA_TYPE_H
#define DATA_TYPE_H

/**
 * @enum DataType
 * @brief Describes all data-types for 'TokenDataType'.
 *
 */
typedef enum {
  TypeInt, /** Type is an integer */
  TypeFloat64, /** Type is a decimal number (float) */
  TypeString /** Type is a text string */
} DataType;

#endif 