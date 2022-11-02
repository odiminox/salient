/* BSD 3-Clause License
 *
 * Copyright © 2008-2022, Jice, Odiminox and the salient contributors.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef SALIENT_STYLESHEET_HPP
#define SALIENT_STYLESHEET_HPP
#include <libtcod/libtcod.hpp>

namespace widget {
/**
 * Specificity levels. Used to determine whether an encountered value can override the one that has been already set.
 */
enum StyleSheetSpecificity {
  /**
   * Default specificity. Used for default values. Can be overridden by anything.
   */
  SPECIFICITY_DEFAULT,
  /**
   * Tag level. This specificity is used when setting a property value for all widgets of a given type, e.g. for all
   * pushbuttons.
   */
  SPECIFICITY_TAG,
  /**
   * Class level. Overrides tag level. Used for setting a common property value for an arbitrary class of widgets (for
   * instance, main menu buttons can belong to a class so that their styling is different from all other pushbuttons).
   */
  SPECIFICITY_CLASS,
  /**
   * ID level. Used for styling individual widgets. Targets only the widget with a specifin name and overrides both tag
   * and class specificity levels.
   */
  SPECIFICITY_ID,
  /**
   * Used when setting a property's value manually within the code (for instance, for dynamic changes). The external
   * style sheet cannot use this specificity. It overrides everything.
   */
  SPECIFICITY_MANUAL
};

class StyleSheetSet;  // forward declaration

/**
 * A template of a stylesheet property, including the property's value and the level at which it has been set.
 */
template <class T>
class StyleSheetProperty {
  friend class StyleSheetSet;

 private:
  /**
   * The property's value.
   */
  T val;
  /**
   * The level at which the property has been set (tag, class, ID or manual). Used to determine the precedence.
   */
  StyleSheetSpecificity specificity;

 public:
  /**
   * Fetches the property's value. Equivalent to the overloaded <code>()</code> operator.
   * @return the property's value
   */
  inline T value() { return val; }
  /**
   * Overloaded assignment operator. Assigns a value to the property.
   * @param x the value to be assigned to the property
   * @return the property object reference
   */
  inline StyleSheetProperty& operator=(const T& x) {
    val = x;
    specificity = SPECIFICITY_MANUAL;
    return *this;
  }
  /**
   * Overloaded function call operator. Assigns a value to the property.
   * @param x the value to be assigned to the property
   * @return the property object reference
   */
  inline StyleSheetProperty& operator()(const T& x) {
    val = x;
    specificity = SPECIFICITY_MANUAL;
    return *this;
  }
  /**
   * Overloaded function call operator. Fetches the property's value. Equivalent to the <code>value()</code> method.
   * @return the property's value
   */
  inline T& operator()() { return val; }

 private:
  /**
   * Sets the property's value and specificity. If
   * @param x the property's value
   * @param l the property's specificity
   * @return the property object reference
   */
  inline StyleSheetProperty& set(const T& x, StyleSheetSpecificity s = SPECIFICITY_DEFAULT) {
    if (s >= specificity || s == SPECIFICITY_DEFAULT) {
      val = x;
      specificity = s;
    }
    return *this;
  }
};

/**
 * The class that holds the information about a styles set. Used internally by the <code>StyleSheet</code> class.
 */
class StyleSheetSet {
 public:
  /**
   * The style sheet set constructor. Assigns default values to all properties.
   */
  StyleSheetSet();
  /**
   * Text foreground colour.
   */
  StyleSheetProperty<TCODColor> colour;
  /**
   * Widget's background colour.
   */
  StyleSheetProperty<TCODColor> backgroundColour;
  /**
   * Widget's border colour.
   */
  StyleSheetProperty<TCODColor> borderColour;
};

/**
 * The style sheet class. It stores the information about a widget's appearance.
 */
class StyleSheet {
 private:
  static TCODParser parser;

 public:  // style sheets
  /**
   * Default style sheet for the widget.
   */
  StyleSheetSet normal;
  /**
   * Style sheet that overrides the default appearance when the mouse cursor is hovering over the widget.
   */
  StyleSheetSet hover;
  /**
   * Style sheet that overrides the default appearance when the mouse cursor is pressed while hovering over the widget.
   */
  StyleSheetSet active;

 public:  // setters
  /**
   * Sets the style sheet's <code>colour</code> property.
   * @param val the property's value
   * @return reference to the style sheet the method is called on. It can be used for chaining.
   */
  StyleSheet& colour(TCODColor val);
  /**
   * Sets the style sheet's <code>backgroundColour</code> property.
   * @param val the property's value
   * @return reference to the style sheet the method is called on. It can be used for chaining.
   */
  StyleSheet& backgroundColour(TCODColor val);
  /**
   * Sets the style sheet's <code>borderColour</code> property.
   * @param val the property's value
   * @return reference to the style sheet the method is called on. It can be used for chaining.
   */
  StyleSheet& borderColour(TCODColor val);

 public:  // ctor
  /**
   * The constructor for the style sheet class. Fills all styles with default values.
   */
  StyleSheet();
};
}  // namespace widget
#endif /* SALIENT_STYLESHEET_HPP */
